#ifdef DEBUG_FEATURES_ENABLE

#include "EditorWorldView.h"

#include "Engine/Assets/PrimitiveGeometry/PrimitiveGeometryLibrary.h"
#include "Engine/Debug/Editor/RemoteObject/RemoteWorldObject.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/TextureResource/TempTexture.h"
#include "Engine/Module/World/Camera/Camera3D.h"
#include "Engine/Module/World/Mesh/StaticMeshInstance.h"

#include <imgui.h>

using namespace std::string_literals;

void EditorWorldView::initialize() {
	cameraInstance = std::make_unique<EditorDebugCamera>();
	cameraInstance->initialize();

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
	//staticMeshDrawManager.make_instancing(0, meshInstance->key_id(), 1024);
	//staticMeshDrawManager.register_instance(meshInstance);
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

	for (auto& executor : primitive | std::views::values) {
		executor->begin();
	}
	cameraInstance->update();
	cameraInstance->update_affine();
	cameraInstance->transfer();
}

void EditorWorldView::set_camera_command() {
	cameraInstance->register_world_projection(2);
	cameraInstance->register_world_lighting(3);
}

void EditorWorldView::draw_lines() {
	cameraInstance->register_world_projection(1);
	for (auto& executor : primitive | std::views::values) {
		executor->draw_command();
	}
}

std::tuple<bool, Vector2, Vector2> EditorWorldView::draw_editor(const TempTexture& texture) {
	Vector2 resultPos;
	Vector2 resultSize;
	if (ImGui::BeginTabItem(std::format("{}##{}", (remoteWorld ? remoteWorld->world_name() : "UnknownWorld"s), (void*)this).c_str())) {
		isSelectTab = true;

		// Imageの位置とサイズを計算
		ImVec2 winSize = ImGui::GetContentRegionAvail();
		ImVec2 winPos = ImGui::GetWindowPos();

		float aspectX = winSize.x / 16;
		float aspectY = winSize.y / 9;

		ImVec2 imgSize = aspectX > aspectY ?
			ImVec2{ winSize.y / 9 * 16, winSize.y } :
			ImVec2{ winSize.x, winSize.x / 16 * 9 };

		ImVec2 cursorPos = {
			(winSize.x - imgSize.x) * 0.5f + winPos.x + ImGui::GetCursorPosX(),
			(winSize.y - imgSize.y) * 0.5f + winPos.y + ImGui::GetCursorPosY()
		};
		ImGui::SetCursorScreenPos(cursorPos);
		resultPos = { cursorPos.x, cursorPos.y };
		resultSize = { imgSize.x, imgSize.y };

		ImGui::Image(
			static_cast<ImTextureID>(texture.get_as_srv()->handle().ptr), imgSize
		);

		ImGui::EndTabItem();
	}
	else {
		isSelectTab = false;
	}

	return { isSelectTab, resultPos, resultSize };
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
