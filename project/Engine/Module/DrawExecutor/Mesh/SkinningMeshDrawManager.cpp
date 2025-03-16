#include "SkinningMeshDrawManager.h"

#include "Engine/Assets/PolygonMesh/PolygonMeshLibrary.h"
#include "Engine/Assets/Animation/Skeleton/SkeletonLibrary.h"

#include <execution>

void SkinningMeshDrawManager::initialize(uint32_t numLayer) {
	drawData.resize(numLayer);
}

void SkinningMeshDrawManager::make_instancing(uint32_t layer, const std::string& meshName, uint32_t maxInstance) {
	if (layer >= drawData.size()) {
		return;
	}
	if (!PolygonMeshLibrary::IsRegistered(meshName) ||
		!SkeletonLibrary::IsRegistered(meshName)) {
		return;
	}

	Data& data = drawData[layer];
	data.executors.try_emplace(
		meshName,
		PolygonMeshLibrary::GetPolygonMesh(meshName),
		SkeletonLibrary::GetSkeleton(meshName),
		maxInstance
	);
}

void SkinningMeshDrawManager::register_instance(Reference<const SkinningMeshInstance> instance) {
	if (!instance && instance->layer() >= drawData.size()) {
		return;
	}

	drawData[instance->layer()].instances.emplace(instance);
}

void SkinningMeshDrawManager::transfer() {
	for (Data& data : drawData) {
		// 全てのExecutorをリセット
		auto depthView = std::views::values(data.executors);
		std::for_each(
			std::execution::par, depthView.begin(), depthView.end(),
			[](SkinningMeshDrawExecutor& executor) {
			executor.begin();
		});
		std::for_each(
			std::execution::par, data.instances.begin(), data.instances.end(),
			[&executors = data.executors](const Reference<const SkinningMeshInstance>& instance) {
			const std::string& id = instance->mesh_id();
			if (!executors.contains(id)) {
				// 選択したMeshIdがExecutorに存在しない
				return;
			}
			executors.at(id).write_to_buffer(instance);
		});
	}
}

void SkinningMeshDrawManager::draw_layer(uint32_t layer) {
	if (layer >= drawData.size()) {
		// Layer外をDrawCallしようとした
		return;
	}

	for (const SkinningMeshDrawExecutor& executor : drawData[layer].executors | std::views::values) {
		executor.draw_command();
	}
}

#ifdef DEBUG_FEATURES_ENABLE
void SkinningMeshDrawManager::debug_gui() {
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
