#ifdef DEBUG_FEATURES_ENABLE

#include "RemoteStaticMeshInstance.h"

#include "Engine/Application/Output.h"
#include "Engine/Assets/PolygonMesh/PolygonMesh.h"
#include "Engine/Assets/PolygonMesh/PolygonMeshLibrary.h"
#include "Engine/Assets/Texture/TextureLibrary.h"

void RemoteStaticMeshInstance::draw_inspector() {
	ImGui::Text("Type : StaticMeshInstance");

	hierarchyName.show_gui();
	ImGui::Separator();
	// Transform
	transform.show_gui();
	ImGui::Separator();

	ImGui::Checkbox("Draw", &isDraw);
	if (PolygonMeshLibrary::MeshListGui(meshName)) {
		default_material();
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
			treeNodeName = "UnknownMaterialName##" + std::to_string(i);
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
}

nlohmann::json RemoteStaticMeshInstance::serialize() const {
	nlohmann::json json;

	return json;
}

void RemoteStaticMeshInstance::default_material() {
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
