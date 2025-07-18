#ifdef DEBUG_FEATURES_ENABLE

#include "EditorSceneView.h"

#include <imgui.h>

#include "../Core/EditorGizmo.h"
#include "EditorHierarchy.h"
#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/TextureResource/ScreenTexture.h"
#include "Engine/GraphicsAPI/DirectX/DxSwapChain/DxSwapChain.h"
#include "Engine/Module/Render/RenderNode/Debug/PrimitiveLine/PrimitiveLineNode.h"
#include "Engine/Module/Render/RenderNode/Forward/Mesh/StaticMeshNodeForward.h"
#include "Engine/Module/World/Light/DirectionalLight/DirectionalLightInstance.h"
#include "../RemoteObject/RemoteWorldObject.h"
#include "Engine/Application/Output.h"

void EditorSceneView::initialize(bool isActive_) {
	isActive = isActive_;
	screenResultTexture.initialize();

	lightInstance = std::make_unique<DirectionalLightInstance>();
	std::shared_ptr<StaticMeshNodeForward> staticMeshNode = std::make_shared<StaticMeshNodeForward>();
	staticMeshNode->initialize();
	staticMeshNode->set_render_target_SC();
	staticMeshNode->set_config(RenderNodeConfig::Default);

	std::shared_ptr<PrimitiveLineNode> primitiveLineNode = std::make_shared<PrimitiveLineNode>();
	primitiveLineNode->initialize();
	primitiveLineNode->set_render_target_SC();
	primitiveLineNode->set_config(RenderNodeConfig::NoClearDepth | RenderNodeConfig::NoClearRenderTarget);

	staticMeshDrawManager.initialize(1);
	directionalLightingExecutor.reinitialize(1);
	renderPath.initialize(
		{ staticMeshNode,primitiveLineNode }
	);
}

void EditorSceneView::setup(Reference<EditorGizmo> gizmo_, Reference<const EditorHierarchy> hierarchy_) {
	gizmo = gizmo_;
	hierarchy = hierarchy_;
}

void EditorSceneView::update() {
	if (worldViews.contains(selectWorldObject)) {
		EditorWorldView& view = worldViews[selectWorldObject].view;

		view.update();
		directionalLightingExecutor.begin();
		directionalLightingExecutor.write_to_buffer(lightInstance);
		staticMeshDrawManager.transfer();
	}
}

void EditorSceneView::draw_scene() {
	if (!selectWorldObject) {
		return;
	}

	if (worldViews.contains(selectWorldObject)) {
		u32 layer = worldViews[selectWorldObject].layer;
		EditorWorldView& view = worldViews[selectWorldObject].view;

		// 描画フェーズ
		renderPath.begin();
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
	if (worldViews.contains(world)) {
		return;
	}
	// 新規にWorldViewを作成
	auto& tmp = worldViews[world];
	tmp.view.initialize();
	tmp.view.setup(world);
	tmp.layer = layerSize;
	++layerSize;
	staticMeshDrawManager.initialize(layerSize);
}

void EditorSceneView::register_mesh(Reference<const RemoteWorldObject> world, Reference<const StaticMeshInstance> instance) {
	if (worldViews.contains(world)) {
		staticMeshDrawManager.make_instancing(worldViews.at(world).layer, instance->key_id(), 1024);
	}
	staticMeshDrawManager.register_instance(instance);
}

void EditorSceneView::write_primitive(Reference<const RemoteWorldObject> world, const std::string& primitiveName, const Affine& affine) {
	if (!worldViews.contains(world)) {
		Warning("");
		return;
	}
	worldViews.at(world).view.register_primitive(primitiveName, affine);
}

std::optional<u32> EditorSceneView::get_layer(Reference<const RemoteWorldObject> world) const {
	if (!worldViews.contains(world)) {
		return std::nullopt;
	}
	return worldViews.at(world).layer;
}

Reference<EditorWorldView> EditorSceneView::get_world_view(Reference<const RemoteWorldObject> worldRef) {
	if (worldViews.contains(worldRef)) {
		return worldViews[worldRef].view;
	}
	return nullptr;
}

Reference<EditorWorldView> EditorSceneView::get_current_world_view() {
	if (worldViews.empty() || !worldViews.contains(selectWorldObject)) {
		return nullptr;
	}
	return worldViews[selectWorldObject].view;
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
	ImGui::BeginTabBar("WorldViewTabBar");

	// Guizmoのために必要
	drawList = ImGui::GetWindowDrawList();
	isHoverWindow = ImGui::IsWindowHovered();

	// 各WorldViewをImGuiに描画
	auto& worldList = hierarchy->world_list();
	for (u32 i = 0; i < worldList.size(); ++i) {
		auto& world = worldList[i];
		if (worldViews.contains(world)) {
			auto [result, pos, size_] = worldViews.at(world).view.draw_editor(screenResultTexture);
			if (result) {
				selectWorldObject = world;
				origin = pos;
				size = size_;
			}
		}
	}

	ImGui::EndTabBar();
	ImGui::End();
}

#endif // DEBUG_FEATURES_ENABLE
