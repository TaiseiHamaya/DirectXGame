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
			if (cache != meshName) {
				EditorCommandInvoker::Execute(std::make_unique<EditorCommandScopeBegin>());

				default_material();

				EditorValueChangeCommandHandler::GenCommand<std::string>(meshName);
				std::swap(cache, meshName);
				EditorValueChangeCommandHandler::End();

				default_material();

				EditorCommandInvoker::Execute(std::make_unique<EditorCommandScopeEnd>());
			}
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
	ValueEditor::show_object<Color3> colorSO{ "Color" };
	ValueEditor::show_object<r32> shininessSO{ "Shininess" };
	for (i32 i = 0; auto& meshMaterial : materials) {
		std::string treeNodeName;
		auto meshData = PolygonMeshLibrary::GetPolygonMesh(meshName)->mesh_data(i);
		if (meshData) {
			treeNodeName = std::format("{}##{}", meshData->materialName, i);
		}
		if (treeNodeName.empty()) {
			treeNodeName = "UnknownMaterialName##" + std::to_string(i);
		}
		if (ImGui::TreeNodeEx(treeNodeName.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAllColumns)) {
			{
				std::string cache = meshMaterial.texture;
				auto result = TextureLibrary::TextureListGui(cache);

				if (result && cache != meshMaterial.texture) {
					EditorValueChangeCommandHandler::GenCommand<std::string>([&, i = i]() -> std::string& { return materials.at(i).texture; });
					std::swap(cache, meshMaterial.texture);
					EditorValueChangeCommandHandler::End();
				}
			}

			Transform2DShowGuiBody("UVTransform", meshMaterial.uvTransform);

			{
				auto result = colorSO.show_gui(meshMaterial.color);
				if (result == 0b01) {
					EditorValueChangeCommandHandler::GenCommand<Color3>([&, i = i]() -> Color3& { return materials.at(i).color; });
				}
				else if (result == 0b10) {
					EditorValueChangeCommandHandler::End();
				}
			}

			if (ImGui::RadioButton("None", meshMaterial.lightingType == LighingType::None)) {
				if (meshMaterial.lightingType != LighingType::None) {
					EditorValueChangeCommandHandler::GenCommand<LighingType>([&, i = i]() -> LighingType& { return materials.at(i).lightingType; });
					meshMaterial.lightingType = LighingType::None;
					EditorValueChangeCommandHandler::End();
				}
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("Lambert", meshMaterial.lightingType == LighingType::Lambert)) {
				if (meshMaterial.lightingType != LighingType::Lambert) {
					EditorValueChangeCommandHandler::GenCommand<LighingType>([&, i = i]() -> LighingType& { return materials.at(i).lightingType; });
					meshMaterial.lightingType = LighingType::Lambert;
					EditorValueChangeCommandHandler::End();
				}
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("Half lambert", meshMaterial.lightingType == LighingType::HalfLambert)) {
				if (meshMaterial.lightingType != LighingType::HalfLambert) {
					EditorValueChangeCommandHandler::GenCommand<LighingType>([&, i = i]() -> LighingType& { return materials.at(i).lightingType; });
					meshMaterial.lightingType = LighingType::HalfLambert;
					EditorValueChangeCommandHandler::End();
				}
			}

			{
				auto result = shininessSO.show_gui(meshMaterial.shininess);
				if (result == 0b01) {
					EditorValueChangeCommandHandler::GenCommand<r32>([&, i = i]() -> r32& { return materials.at(i).shininess; });
				}
				else if (result == 0b10) {
					EditorValueChangeCommandHandler::End();
				}
			}

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
		jMaterial["Color"] = material.color;
		jMaterial["UV Transform"] = material.uvTransform;
		jMaterial["LightingType"] = static_cast<std::underlying_type_t<LighingType>>(material.lightingType);
		jMaterial["Shininess"] = material.shininess;
		json["Materials"].emplace_back(std::move(jMaterial));
	}

	return json;
}

void RemoteStaticMeshInstance::default_material() {
	std::shared_ptr<const PolygonMesh> mesh = PolygonMeshLibrary::GetPolygonMesh(meshName);

	EditorCommandInvoker::Execute(std::make_unique<EditorCommandResizeContainer<std::vector<Material>>>(
		materials, mesh->material_count()
	));

	for (i32 i = 0; auto& meshMaterial : materials) {
		// 色情報のリセット
		const auto* meshMaterialData = mesh->material_data(i);
		if (meshMaterialData) {
			{
				EditorValueChangeCommandHandler::GenCommand<std::string>([&, i = i]() -> std::string& { return materials.at(i).texture; });
				meshMaterial.texture = meshMaterialData->textureFileName;
				EditorValueChangeCommandHandler::End();
			}
			EditorValueChangeCommandHandler::GenCommand<Vector2>([&, i = i]() -> Vector2& { return materials.at(i).uvTransform.get_scale(); });
			meshMaterial.uvTransform.set_scale(meshMaterialData->defaultUV.get_scale());
			EditorValueChangeCommandHandler::End();
			EditorValueChangeCommandHandler::GenCommand<r32>([&, i = i]() -> r32& { return materials.at(i).uvTransform.get_rotate(); });
			meshMaterial.uvTransform.set_rotate(meshMaterialData->defaultUV.get_rotate());
			EditorValueChangeCommandHandler::End();
			EditorValueChangeCommandHandler::GenCommand<Vector2>([&, i = i]() -> Vector2& { return materials.at(i).uvTransform.get_translate(); });
			meshMaterial.uvTransform.set_translate(meshMaterialData->defaultUV.get_translate());
			EditorValueChangeCommandHandler::End();

		}
		else {
			{
				EditorValueChangeCommandHandler::GenCommand<std::string>([&, i = i]() -> std::string& { return materials.at(i).texture; });
				meshMaterial.texture = "Error.png";
				EditorValueChangeCommandHandler::End();
			}
			EditorValueChangeCommandHandler::GenCommand<Vector2>([&, i = i]() -> Vector2& { return materials.at(i).uvTransform.get_scale(); });
			meshMaterial.uvTransform.set_scale(CVector2::ONE);
			EditorValueChangeCommandHandler::End();
			EditorValueChangeCommandHandler::GenCommand<r32>([&, i = i]() -> r32& { return materials.at(i).uvTransform.get_rotate(); });
			meshMaterial.uvTransform.set_rotate(0);
			EditorValueChangeCommandHandler::End();
			EditorValueChangeCommandHandler::GenCommand<Vector2>([&, i = i]() -> Vector2& { return materials.at(i).uvTransform.get_translate(); });
			meshMaterial.uvTransform.set_translate(CVector2::ZERO);
			EditorValueChangeCommandHandler::End();

			Warning("Material data is not found.");
		}
		{
			EditorValueChangeCommandHandler::GenCommand<LighingType>([&, i = i]() -> LighingType& { return materials.at(i).lightingType; });
			meshMaterial.lightingType = LighingType::HalfLambert;
			EditorValueChangeCommandHandler::End();
		}
		{
			EditorValueChangeCommandHandler::GenCommand<Color3>([&, i = i]() -> Color3& { return materials.at(i).color; });
			meshMaterial.color = CColor3::WHITE;
			EditorValueChangeCommandHandler::End();
		}
		{
			EditorValueChangeCommandHandler::GenCommand<r32>([&, i = i]() -> r32& { return materials.at(i).shininess; });
			meshMaterial.shininess = 50;
			EditorValueChangeCommandHandler::End();
		}

		++i;
	}
}

#endif // DEBUG_FEATURES_ENABLE
