#include "StaticMeshInstance.h"

#include "Engine/Application/Output.h"
#include "Engine/Assets/PolygonMesh/PolygonMesh.h"
#include "Engine/Assets/PolygonMesh/PolygonMeshLibrary.h"
#include "Engine/Assets/Texture/TextureLibrary.h"
#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/BufferObjects.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/Texture/Texture.h"

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

StaticMeshInstance::StaticMeshInstance() noexcept(false) :
	WorldInstance() {
	materials.clear();
}

StaticMeshInstance::StaticMeshInstance(const std::string& meshName_) noexcept(false) :
	StaticMeshInstance() {
	reset_mesh(meshName_);
}

StaticMeshInstance::~StaticMeshInstance() noexcept = default;

StaticMeshInstance::StaticMeshInstance(StaticMeshInstance&&) noexcept = default;

StaticMeshInstance& StaticMeshInstance::operator=(StaticMeshInstance&&) noexcept = default;

void StaticMeshInstance::reset_mesh(const std::string& meshName_) {
	// メッシュ情報の取得
	if (PolygonMeshLibrary::IsRegistered(meshName_)) {
		meshName = meshName_;
	}
	else {
		meshName = "ErrorObject.obj";
	}

#ifdef _DEBUG
	mesh = PolygonMeshLibrary::GetPolygonMesh(meshName);
#endif // _DEBUG


	default_material();
}

void StaticMeshInstance::default_material() {
	std::shared_ptr<const PolygonMesh> mesh = PolygonMeshLibrary::GetPolygonMesh(meshName);

	materials.resize(mesh->material_count());

	for (int i = 0; auto & meshMaterial : materials) {
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

std::vector<StaticMeshInstance::Material>& StaticMeshInstance::get_materials() {
	return materials;
}

const std::vector<StaticMeshInstance::Material>& StaticMeshInstance::get_materials() const {
	return materials;
}

bool StaticMeshInstance::is_draw() const {
	return isDraw && isActive;
}

void StaticMeshInstance::set_texture(const std::string& name, int index) {
	materials[index].texture = TextureLibrary::GetTexture(name);
}

#ifdef _DEBUG
void StaticMeshInstance::debug_gui() {
	if (PolygonMeshLibrary::MeshListGui(meshName)) {
		reset_mesh(meshName);
	}
	if (ImGui::Button("ResetMaterialData")) {
		default_material();
	}
	ImGui::Checkbox("Draw", &isDraw);
	ImGui::Separator();
	WorldInstance::debug_gui();
	ImGui::Separator();
	ImGui::Text("Materials");
	for (int i = 0; auto & meshMaterial : materials) {
		std::string treeNodeName;
		auto meshData = mesh->mesh_data(i);
		if (meshData) {
			treeNodeName = meshData->materialName + "##" + std::to_string(i);
		}
		if (treeNodeName.empty()) {
			treeNodeName = "UnknownMaterialName##" + std::to_string(i);
		}
		std::string textureName = meshMaterial.texture->name();
		if (ImGui::TreeNodeEx(treeNodeName.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
			if (TextureLibrary::TextureListGui(textureName)) {
				set_texture(textureName, i);
			}

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

			ImGui::DragFloat("Shininess", &meshMaterial.shininess, 0.1f, 0.0f, std::numeric_limits<float>::max());

			ImGui::TreePop();
		}
		++i;
	}
}
#endif // _DEBUG
