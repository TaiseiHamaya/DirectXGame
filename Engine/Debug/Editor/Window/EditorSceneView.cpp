#ifdef DEBUG_FEATURES_ENABLE

#include "EditorSceneView.h"

#include <imgui.h>

#include "../Core/EditorGizmo.h"
#include "../RemoteObject/RemoteWorldObject.h"
#include "EditorHierarchy.h"
#include "Engine/Application/Logger.h"
#include "Engine/Application/ProjectSettings/ProjectSettings.h"
#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/TextureResource/ScreenTexture.h"
#include "Engine/GraphicsAPI/DirectX/DxSwapChain/DxSwapChain.h"
#include "Engine/Module/Render/RenderPSO/Debug/PrimitiveLine/PrimitiveLineNode.h"
#include "Engine/Module/Render/RenderPSO/Forward/Mesh/StaticMeshNodeForward.h"
#include "Engine/Module/Render/RenderTargetGroup/SwapChainRenderTargetGroup.h"
#include "Engine/Module/World/Light/DirectionalLight/DirectionalLightInstance.h"

void EditorSceneView::initialize(bool isActive_) {
	isActive = isActive_;
	screenResultTexture.initialize();

	lightInstance = std::make_unique<DirectionalLightInstance>();
	std::shared_ptr<StaticMeshNodeForward> staticMeshNode = std::make_shared<StaticMeshNodeForward>();
	staticMeshNode->initialize();

	std::shared_ptr<PrimitiveLineNode> primitiveLineNode = std::make_shared<PrimitiveLineNode>();
	primitiveLineNode->initialize();

	staticMeshDrawManager.initialize(1);
	directionalLightingExecutor.reinitialize(1);
	renderPath.initialize(
		{ staticMeshNode,primitiveLineNode }
	);

	EditorDebugCamera::Setup(this);
}

void EditorSceneView::setup(Reference<EditorGizmo> gizmo_, Reference<const EditorHierarchy> hierarchy_) {
	gizmo = gizmo_;
	hierarchy = hierarchy_;
}

void EditorSceneView::update() {
	if (selectWorldId.has_value() && worldViews.contains(selectWorldId.value())) {
		EditorWorldView& view = worldViews[selectWorldId.value()].view;

		view.update();
		directionalLightingExecutor.begin();
		directionalLightingExecutor.write_to_buffer(lightInstance);
		staticMeshDrawManager.transfer();
	}
}

void EditorSceneView::draw_scene() {
	if (!selectWorldId.has_value()) {
		return;
	}

	if (worldViews.contains(selectWorldId.value())) {
		u32 layer = worldViews[selectWorldId.value()].layer;
		EditorWorldView& view = worldViews[selectWorldId.value()].view;

		// 描画フェーズ
		renderPath.begin();
		auto swapChainBuffer = DxSwapChain::GetRenderTarget();
		auto depthStencilTexture = RenderingSystemValues::GetDepthStencilTexture();
		swapChainBuffer->begin_write(true, depthStencilTexture);
		depthStencilTexture->start_write();
		depthStencilTexture->get_as_dsv()->clear();
		auto&& commandList = DxCommand::GetCommandList();
		D3D12_VIEWPORT viewPort{
			0.0f, 0.0f,
			ProjectSettings::ClientSize().x, ProjectSettings::ClientSize().y,
			0.0f, 1.0f
		};
		RECT scissorRect = D3D12_RECT{
			0, 0, (long)ProjectSettings::ClientWidth(), (long)ProjectSettings::ClientHeight()
		};

		// ViewPortの設定
		commandList->RSSetViewports(1, &viewPort);
		// シザー矩形の設定
		commandList->RSSetScissorRects(1, &scissorRect);

		// Mesh
		view.set_camera_command();
		directionalLightingExecutor.set_command(4);
		staticMeshDrawManager.draw_layer(layer);

		renderPath.next();
		// lines
		view.draw_lines();

		renderPath.next();
	}
	copy_screen();
}

void EditorSceneView::draw() {
	set_imgui_command();
}

bool EditorSceneView::is_hovered_window() {
	return
		(isActive && isHoverWindow) ||
		!ImGui::GetIO().WantCaptureMouse;
}

const Vector2& EditorSceneView::view_origin() const {
	return origin;
}

const Vector2& EditorSceneView::view_size() const {
	return size;
}

Reference<ImDrawList> EditorSceneView::draw_list() const {
	return drawList;
}

void EditorSceneView::register_world(Reference<RemoteWorldObject> world) {
	if (!world) {
		return;
	}
	if (worldViews.contains(world->get_id())) {
		return;
	}
	// 新規にWorldViewを作成
	auto& tmp = worldViews[world->get_id()];
	tmp.view.initialize();
	tmp.view.setup(world);
	tmp.layer = layerSize;
	++layerSize;
	staticMeshDrawManager.initialize(layerSize);
}

void EditorSceneView::create_mesh_instancing(Reference<const RemoteWorldObject> world, const std::string& meshName) {
	if (worldViews.contains(world->get_id())) {
		staticMeshDrawManager.make_instancing(worldViews.at(world->get_id()).layer, meshName, 1024);
	}
}

void EditorSceneView::register_mesh(Reference<const RemoteWorldObject> world, Reference<const StaticMeshInstance> instance) {
	create_mesh_instancing(world, instance->key_id());
	staticMeshDrawManager.register_instance(instance);
}

void EditorSceneView::write_primitive(Reference<const RemoteWorldObject> world, const std::string& primitiveName, const Affine& affine) {
	if (!worldViews.contains(world->get_id())) {
		szgWarning("");
		return;
	}
	worldViews.at(world->get_id()).view.register_primitive(primitiveName, affine);
}

std::optional<u32> EditorSceneView::get_layer(Reference<const RemoteWorldObject> world) const {
	if (!worldViews.contains(world->get_id())) {
		return std::nullopt;
	}
	return worldViews.at(world->get_id()).layer;
}

Reference<EditorWorldView> EditorSceneView::get_world_view(Reference<const RemoteWorldObject> world) {
	if (!worldViews.contains(world->get_id())) {
		return nullptr;
	}
	return worldViews.at(world->get_id()).view;
}

Reference<EditorWorldView> EditorSceneView::get_current_world_view() {
	if (worldViews.empty() || !selectWorldId.has_value() || !worldViews.contains(selectWorldId.value())) {
		return nullptr;
	}
	return worldViews[selectWorldId.value()].view;
}

void EditorSceneView::reset_force() {
	selectWorldId.reset();
	worldViews.clear();
	staticMeshDrawManager = StaticMeshDrawManager{};
	layerSize = 0;
}

void EditorSceneView::copy_screen() {
	auto& command = DxCommand::GetCommandList();
	Reference<ScreenTexture> screen = DxSwapChain::GetWriteBufferTexture();

	screenResultTexture.start_copy_dest();
	screen->start_read();

	command->CopyResource(screenResultTexture.get_resource().Get(), screen->get_resource().Get());
}

void EditorSceneView::set_imgui_command() {
	if (!isActive) {
		return;
	}
	if (isHoverWindow && (ImGui::GetIO().MouseDown[1] || ImGui::GetIO().MouseDown[2])) {
		ImGui::SetNextWindowFocus();
	}

	screenResultTexture.start_read();
	ImGui::Begin("Scene", &isActive, ImGuiWindowFlags_NoScrollbar);

	// Gizmo用ヘッダー描画
	gizmo->scene_header();
	ImGui::Separator();
	ImGui::BeginTabBar("WorldViewTabBar", ImGuiTabBarFlags_DrawSelectedOverline);

	// Guizmoのために必要
	drawList = ImGui::GetWindowDrawList();
	isHoverWindow = ImGui::IsWindowHovered();

	// 各WorldViewをImGuiに描画
	auto& worldList = hierarchy->world_list();
	for (u32 i = 0; i < worldList.size(); ++i) {
		u32 world = worldList[i]->get_id();
		if (worldViews.contains(world)) {
			auto [result, pos, size_] = worldViews.at(world).view.draw_editor(screenResultTexture);
			if (result) {
				selectWorldId = world;
				origin = pos;
				size = size_;
			}
		}
	}

	ImGui::EndTabBar();
	ImGui::End();
}

#endif // DEBUG_FEATURES_ENABLE
