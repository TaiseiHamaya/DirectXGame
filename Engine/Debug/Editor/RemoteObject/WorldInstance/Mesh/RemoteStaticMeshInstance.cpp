#ifdef DEBUG_FEATURES_ENABLE

#include "RemoteStaticMeshInstance.h"

#include "Engine/Application/Output.h"
#include "Engine/Assets/PolygonMesh/PolygonMesh.h"
#include "Engine/Assets/PolygonMesh/PolygonMeshLibrary.h"
#include "Engine/Assets/Texture/TextureLibrary.h"
#include "Engine/Debug/Editor/Command/EditorCommandResizeContainer.h"

void RemoteStaticMeshInstance::draw_inspector() {
	ImGui::Text("Type : StaticMeshInstance");

	hierarchyName.show_gui();
	ImGui::Separator();
	// Transform
	transform.show_gui();
	ImGui::Separator();

	isDraw.show_gui();
	layer.show_gui();
	{
		std::string cache = meshName;
		if (PolygonMeshLibrary::MeshListGui(cache)) {
			EditorCommandInvoker::Execute(std::make_unique<EditorCommandScopeBegin>());

			EditorValueChangeCommandHandler::GenCommand<std::string>(meshName);
			std::swap(cache, meshName);
			EditorValueChangeCommandHandler::End();

			default_material();

			EditorCommandInvoker::Execute(std::make_unique<EditorCommandScopeEnd>());
		}
	}
	if (ImGui::Button("ResetMaterialData")) {
		EditorCommandInvoker::Execute(std::make_unique<EditorCommandScopeBegin>());
		default_material();
		EditorCommandInvoker::Execute(std::make_unique<EditorCommandScopeEnd>());
	}

	ImGui::Separator();
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
			{
				std::string cache = meshMaterial.texture;
				auto result = TextureLibrary::TextureListGui(cache);

				if (result) {
					EditorValueChangeCommandHandler::GenCommand<std::string>(meshMaterial.texture);
					std::swap(cache, meshMaterial.texture);
					EditorValueChangeCommandHandler::End();
				}
			}

			meshMaterial.uvTransform.show_gui();

			meshMaterial.color.show_gui();

			if (ImGui::RadioButton("None", meshMaterial.lightingType == LighingType::None)) {
				EditorValueChangeCommandHandler::GenCommand<LighingType>(meshMaterial.lightingType);
				meshMaterial.lightingType = LighingType::None;
				EditorValueChangeCommandHandler::End();
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("Lambert", meshMaterial.lightingType == LighingType::Lambert)) {
				EditorValueChangeCommandHandler::GenCommand<LighingType>(meshMaterial.lightingType);
				meshMaterial.lightingType = LighingType::Lambert;
				EditorValueChangeCommandHandler::End();
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("Half lambert", meshMaterial.lightingType == LighingType::HalfLambert)) {
				EditorValueChangeCommandHandler::GenCommand<LighingType>(meshMaterial.lightingType);
				meshMaterial.lightingType = LighingType::HalfLambert;
				EditorValueChangeCommandHandler::End();
			}

			meshMaterial.shininess.show_gui();

			ImGui::TreePop();
		}
		++i;
	}
}

nlohmann::json RemoteStaticMeshInstance::serialize() const {
	nlohmann::json json;

	json.update(hierarchyName);
	json.update(transform);
	json["Type"] = 10;
	json.update(isDraw);
	json.update(layer);
	json["MeshName"] = meshName;
	json["Materials"] = nlohmann::json::array();
	for (const auto& material : materials) {
		nlohmann::json jMaterial;
		jMaterial["Texture"] = material.texture;
		jMaterial.update(material.color);
		jMaterial.update(material.uvTransform);
		jMaterial["LightingType"] = static_cast<std::underlying_type_t<LighingType>>(material.lightingType);
		jMaterial.update(material.shininess);
		json["Materials"].emplace_back(std::move(jMaterial));
	}

	return json;
}

void RemoteStaticMeshInstance::default_material() {
	std::shared_ptr<const PolygonMesh> mesh = PolygonMeshLibrary::GetPolygonMesh(meshName);

	EditorCommandInvoker::Execute(std::make_unique<EditorCommandResizeContainer<std::deque<Material>>>(
		materials, mesh->material_count()
	));

	for (i32 i = 0; auto& meshMaterial : materials) {
		// 色情報のリセット
		const auto* meshMaterialData = mesh->material_data(i);
		if (meshMaterialData) {
			{
				EditorValueChangeCommandHandler::GenCommand<std::string>(meshMaterial.texture);
				meshMaterial.texture = meshMaterialData->textureFileName;
				EditorValueChangeCommandHandler::End();
			}
			meshMaterial.uvTransform.set(meshMaterialData->defaultUV);

		}
		else {
			meshMaterial.texture = "Error.png";
			meshMaterial.uvTransform.set(Transform2D{});
			Warning("Material data is not found.");
		}
		{
			EditorValueChangeCommandHandler::GenCommand<LighingType>(meshMaterial.lightingType);
			meshMaterial.lightingType = LighingType::HalfLambert;
			EditorValueChangeCommandHandler::End();
		}
		meshMaterial.color.set(CColor3::WHITE);
		meshMaterial.shininess.set(50);

		++i;
	}
}

#endif // DEBUG_FEATURES_ENABLE
