#ifdef DEBUG_FEATURES_ENABLE

#include "RemoteSkinningMeshInstance.h"

#include "Engine/Application/Output.h"
#include "Engine/Assets/Animation/Skeleton/SkeletonAsset.h"
#include "Engine/Assets/Animation/Skeleton/SkeletonLibrary.h"
#include "Engine/Assets/PolygonMesh/PolygonMesh.h"
#include "Engine/Assets/PolygonMesh/PolygonMeshLibrary.h"
#include "Engine/Assets/Texture/TextureAsset.h"
#include "Engine/Assets/Texture/TextureLibrary.h"
#include "Engine/Debug/Editor/Command/EditorValueChangeCommandHandler.h"

void RemoteSkinningMeshInstance::draw_inspector() {
	ImGui::Text("Type : SkinningMeshInstance");

	hierarchyName.show_gui();
	ImGui::Separator();
	// Transform
	transform.show_gui();
	ImGui::Separator();

	ImGui::Checkbox("Draw", &isDraw);
	if (PolygonMeshLibrary::MeshListGui(meshName)) {
		default_material();
		animationName = "UnknownAnimation";
	}
	if (ImGui::Button("ResetMaterialData")) {
		default_material();
	}

	// Material
	ImGui::Text("Materials");
	for (i32 i = 0; auto& meshMaterial : materials) {
		std::string treeNodeName;
		auto meshData = PolygonMeshLibrary::GetPolygonMesh(meshName)->mesh_data(i);
		if (meshData) {
			treeNodeName = std::format("{}##{}", meshData->materialName, i);
		}
		if (treeNodeName.empty()) {
			treeNodeName = std::format("UnknownMaterialName##{}", i);
		}
		if (ImGui::TreeNodeEx(treeNodeName.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
			TextureLibrary::TextureListGui(meshMaterial.texture);

			meshMaterial.uvTransform.debug_gui();

			meshMaterial.color.debug_gui();

			if (ImGui::RadioButton("None", meshMaterial.lightingType == LighingType::None)) {
				meshMaterial.lightingType = LighingType::None;
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("Lambert", meshMaterial.lightingType == LighingType::Lambert)) {
				meshMaterial.lightingType = LighingType::Lambert;
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("Half lambert", meshMaterial.lightingType == LighingType::HalfLambert)) {
				meshMaterial.lightingType = LighingType::HalfLambert;
			}

			ImGui::DragFloat("Shininess", &meshMaterial.shininess, 0.1f, 0.0f, std::numeric_limits<r32>::max());

			ImGui::TreePop();
		}
		++i;
	}
	ImGui::Separator();

	//ここからAnimation専用処理
	ImGui::InputText("Animation Name", &animationName);
	ImGui::Checkbox("Loop", &isLoop);
	ImGui::Separator();

	// Skeleton
	std::shared_ptr<const SkeletonAsset> skeleton = SkeletonLibrary::GetSkeleton(animationName);
	if (skeleton) {
		ImGui::Text("NumSkeleton : %d", skeleton->joint_size());
	}
	else {
		ImGui::Text("Warning: Skeleton is not found.");
	}
}

nlohmann::json RemoteSkinningMeshInstance::serialize() const {
	return nlohmann::json();
}

void RemoteSkinningMeshInstance::default_material() {
	std::shared_ptr<const PolygonMesh> mesh = PolygonMeshLibrary::GetPolygonMesh(meshName);

	materials.resize(mesh->material_count());

	for (i32 i = 0; auto& meshMaterial : materials) {
		// 色情報のリセット
		const auto* meshMaterialData = mesh->material_data(i);
		if (meshMaterialData) {
			// テクスチャ情報の取得
			meshMaterial.texture = TextureLibrary::GetTexture(meshMaterialData->textureFileName);
			// uv情報のリセット
			meshMaterial.uvTransform.copy(meshMaterialData->defaultUV);
		}
		else {
			meshMaterial.texture = TextureLibrary::GetTexture("Error.png");
			meshMaterial.uvTransform.copy(Transform2D{});
			Warning("Material data is not found.");
		}
		++i;
	}
}

#endif // DEBUG_FEATURES_ENABLE
