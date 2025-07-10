#ifdef DEBUG_FEATURES_ENABLE

#include "EditorWorldView.h"

#include "Engine/Assets/PrimitiveGeometry/PrimitiveGeometryLibrary.h"
#include "Engine/Debug/Editor/RemoteObject/RemoteWorldObject.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/TextureResource/TempTexture.h"
#include "Engine/Module/Render/RenderNode/Forward/Mesh/StaticMeshNodeForward.h"
#include "Engine/Module/Render/RenderNode/Debug/PrimitiveLine/PrimitiveLineNode.h"
#include "Engine/Module/World/Camera/Camera3D.h"
#include "Engine/Module/World/Light/DirectionalLight/DirectionalLightInstance.h"
#include "Engine/Module/World/Mesh/StaticMeshInstance.h"

#include <imgui.h>

using namespace std::string_literals;

void EditorWorldView::initialize() {
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
	renderPath.initialize({
		staticMeshNode,primitiveLineNode
		});

	cameraInstance = std::make_unique<EditorDebugCamera>();
	cameraInstance->initialize();
	lightInstance = std::make_unique<DirectionalLightInstance>();

	primitive.emplace("Frustum", std::make_unique<PrimitiveGeometryDrawExecutor>(
		PrimitiveGeometryLibrary::GetPrimitiveGeometry("Frustum"), 16
	));
	primitive.emplace("AABBCollider", std::make_unique<PrimitiveGeometryDrawExecutor>(
		PrimitiveGeometryLibrary::GetPrimitiveGeometry("AABBCollider"), 1024
	));
	primitive.emplace("SphereCollider", std::make_unique<PrimitiveGeometryDrawExecutor>(
		PrimitiveGeometryLibrary::GetPrimitiveGeometry("SphereCollider"), 1024
	));
}

void EditorWorldView::setup(Reference<RemoteWorldObject> remoteWorld_) {
	remoteWorld = remoteWorld_;
}

void EditorWorldView::register_mesh(Reference<StaticMeshInstance> meshInstance) {
	staticMeshDrawManager.make_instancing(0, meshInstance->key_id(), 1024);
	staticMeshDrawManager.register_instance(meshInstance);
}

void EditorWorldView::register_primitive(const std::string& name, const Affine& affine) {
	if (primitive.contains(name)) {
		primitive.at(name)->write_to_buffer(affine.to_matrix());
	}
}

void EditorWorldView::update() {
	if (!isSelectTab) {
		return;
	}

	directionalLightingExecutor.begin();
	for (auto& executor : primitive | std::views::values) {
		executor->begin();
	}
	cameraInstance->update();
}

void EditorWorldView::rendering() {
	if (!isSelectTab) {
		return;
	}

	// 転送処理
	staticMeshDrawManager.transfer();
	directionalLightingExecutor.write_to_buffer(lightInstance);
	cameraInstance->update_affine();
	cameraInstance->transfer();

	// 描画処理
	renderPath.begin();
	// StaticMesh
	cameraInstance->register_world_projection(2);
	cameraInstance->register_world_lighting(3);
	directionalLightingExecutor.set_command(4);
	staticMeshDrawManager.draw_layer(0);

	renderPath.next();
	cameraInstance->register_world_projection(1);
	for (auto& executor : primitive | std::views::values) {
		executor->draw_command();
	}

	renderPath.next(); // End
}

bool EditorWorldView::draw_editor(const TempTexture& texture, r32 sizeX, r32 sizeY) {
	if (ImGui::BeginTabItem(std::format("{}##{}", (remoteWorld ? remoteWorld->world_name() : "UnknownWorld"s), (void*)this).c_str())) {
		isSelectTab = true;

		ImGui::Image(
			static_cast<ImTextureID>(texture.get_as_srv()->handle().ptr), ImVec2{ sizeX, sizeY }
		);

		ImGui::EndTabItem();
	}
	else {
		isSelectTab = false;
	}

	return isSelectTab;
}

void EditorWorldView::camera_gui() {
	cameraInstance->debug_gui();
}

Reference<const Camera3D> EditorWorldView::get_camera() const {
	return cameraInstance;
}

bool EditorWorldView::is_select_tab() const {
	return isSelectTab;
}

Reference<RemoteWorldObject> EditorWorldView::remote_world() const {
	return remoteWorld;
}

#endif // DEBUG_FEATURES_ENABLE
