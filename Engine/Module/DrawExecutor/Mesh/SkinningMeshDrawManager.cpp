#include "SkinningMeshDrawManager.h"

#include "Engine/Assets/PolygonMesh/PolygonMeshLibrary.h"
#include "Engine/Assets/Animation/Skeleton/SkeletonLibrary.h"

void SkinningMeshDrawManager::make_instancing(u32 layer, const std::string& meshName, u32 maxInstance) {
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

#ifdef DEBUG_FEATURES_ENABLE

#include <imgui.h>

void SkinningMeshDrawManager::debug_gui() {
	PolygonMeshLibrary::MeshListGui(select);

	i32 step = 1;
	ImGui::PushItemWidth(80);
	ImGui::InputScalar("Layer", ImGuiDataType_U32, &layer, (void*)&step);  ImGui::SameLine();
	ImGui::InputScalar("MaxInstance", ImGuiDataType_U32, &maxInstance, (void*)&step);
	ImGui::PopItemWidth();

	if (ImGui::Button("Apply") && maxInstance >= 1) {
		make_instancing(layer, select, maxInstance);
	}

	for (u32 i = 0; auto & data : drawData) {
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
