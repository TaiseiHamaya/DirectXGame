#include "StaticMeshDrawManager.h"

#include "Engine/Assets/PolygonMesh/PolygonMeshLibrary.h"

#include <execution>

void StaticMeshDrawManager::initialize(uint32_t numLayer) {
	drawData.resize(numLayer);
}

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

void StaticMeshDrawManager::register_instance(Reference<const StaticMeshInstance> instance) {
	if (!instance && instance->layer() >= drawData.size()) {
		return;
	}

	drawData[instance->layer()].instances.emplace(instance);
}

void StaticMeshDrawManager::transfer() {
	for (Data& data : drawData) {
		// 全てのExecutorをリセット
		auto depthView = std::views::values(data.executors);
		std::for_each(
			std::execution::par, depthView.begin(), depthView.end(),
			[](StaticMeshDrawExecutor& executor) {
			executor.begin();
		});
		std::for_each(
			std::execution::par, data.instances.begin(), data.instances.end(),
			[&executors = data.executors](const Reference<const StaticMeshInstance>& instance) {
			const std::string& id = instance->mesh_id();
			if (!executors.contains(id)) {
				// 選択したMeshIdがExecutorに存在しない
				return;
			}
			auto& executor = executors[id];
			executor.write_to_buffer(instance);
		});
	}
}

void StaticMeshDrawManager::draw_layer(uint32_t layer) {
	if (layer >= drawData.size()) {
		// Layer外をDrawCallしようとした
		return;
	}

	for (const StaticMeshDrawExecutor& executor : drawData[layer].executors | std::views::values) {
		executor.draw_command();
	}
}

#ifdef _DEBUG
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
	PolygonMeshLibrary::MeshListGui(selectMesh);

	int step = 1;
	ImGui::PushItemWidth(80);
	ImGui::InputScalar("Layer", ImGuiDataType_U32, &layer, (void*)&step);  ImGui::SameLine();
	ImGui::InputScalar("MaxInstance", ImGuiDataType_U32, &maxInstance, (void*)&step);
	ImGui::PopItemWidth();

	if (ImGui::Button("Apply") && maxInstance >= 1) {
		make_instancing(layer, selectMesh, maxInstance);
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
