#include "StaticMeshDrawManager.h"

#include "Engine/Assets/PolygonMesh/PolygonMeshLibrary.h"

#include <execution>

void StaticMeshDrawManager::make_instancing(uint32_t layer, const std::string& meshName_, uint32_t maxInstance) {
	if (layer >= drawData.size()) {
		return;
	}
	if (!PolygonMeshLibrary::IsRegistered(meshName_)) {
		return;
	}

	Data& data = drawData[layer];
	data.executors.try_emplace(
		meshName_,
		PolygonMeshLibrary::GetPolygonMesh(meshName_), maxInstance
	);
}

#ifdef DEBUG_FEATURES_ENABLE
#include <Engine/Module/World/Camera/Camera3D.h>
#include <Engine/Debug/DebugValues/DebugValues.h>

void StaticMeshDrawManager::register_debug_instance(uint32_t layer, Reference<const Camera3D> camera, bool isShowGrid) {
	if (layer >= drawData.size()) {
		return;
	}

	Data& data = drawData[layer];

	data.executors.try_emplace(
		"CameraAxis.obj",
		PolygonMeshLibrary::GetPolygonMesh("CameraAxis.obj"), 1
	);
	data.executors.try_emplace(
		"Grid.obj",
		PolygonMeshLibrary::GetPolygonMesh("Grid.obj"), 1
	);

	if (camera) {
		data.instances.emplace(camera->camera_axis());
	}

	if (isShowGrid) {
		data.instances.emplace(DebugValues::GetGridInstance());
	}
}

#include <imgui.h>
void StaticMeshDrawManager::debug_gui() {
	PolygonMeshLibrary::MeshListGui(select);

	int step = 1;
	ImGui::PushItemWidth(80);
	ImGui::InputScalar("Layer", ImGuiDataType_U32, &layer, (void*)&step);  ImGui::SameLine();
	ImGui::InputScalar("MaxInstance", ImGuiDataType_U32, &maxInstance, (void*)&step);
	ImGui::PopItemWidth();

	if (ImGui::Button("Apply") && maxInstance >= 1) {
		make_instancing(layer, select, maxInstance);
	}

	for (uint32_t i = 0; auto & data : drawData) {
		if (ImGui::TreeNode(std::format("Layer{}", i).c_str())) {
			ImGui::Text(std::format("RegisteredInstance : {}", data.instances.size()).c_str());
			ImGui::Indent();
			for (const auto& [name, executor] : data.executors) {
				ImGui::Text(std::format("{} : {}/{}", name, executor.count(), executor.max_instance()).c_str());
			}
			ImGui::Unindent();
			ImGui::TreePop();
		}
		++i;
	}
}
#endif // _DEBUG
