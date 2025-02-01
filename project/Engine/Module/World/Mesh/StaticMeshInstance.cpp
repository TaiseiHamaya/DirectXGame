#include "StaticMeshInstance.h"

#include <Library/Utility/Tools/SmartPointer.h>

#include "Engine/Assets/PolygonMesh/PolygonMesh.h"
#include "Engine/Assets/PolygonMesh/PolygonMeshLibrary.h"
#include "Engine/Assets/Texture/TextureLibrary.h"
#include "Engine/Debug/Output.h"
#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/ConstantBuffer/Material/Material.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/ConstantBuffer/TransformMatrix/TransformMatrix.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/Texture/Texture.h"


#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

StaticMeshInstance::StaticMeshInstance() noexcept(false) :
	WorldInstance(),
	// 各メモリの取得
	transformMatrix(std::make_unique<TransformMatrix>()) {
	meshMaterials.clear();
}

StaticMeshInstance::StaticMeshInstance(const std::string& meshName_) noexcept(false) :
	StaticMeshInstance() {
	reset_mesh(meshName_);
}

StaticMeshInstance::~StaticMeshInstance() noexcept = default;

StaticMeshInstance::StaticMeshInstance(StaticMeshInstance&&) noexcept = default;

StaticMeshInstance& StaticMeshInstance::operator=(StaticMeshInstance&&) noexcept = default;

void StaticMeshInstance::transfer() noexcept {
	if (!isActive) {
		return;
	}
	// 各情報をGPUに転送
	// Transformに転送
	transformMatrix->set_transformation_matrix_data(world_affine());
	// Materialに転送
	for (PolygonMeshMaterial& material : meshMaterials) {
		material.write_buffer();
	}
}

void StaticMeshInstance::draw() const {
	// 非アクティブ時は描画しない
	if (!isActive || !isDraw) {
		return;
	}
	// メッシュが存在しなければ描画しない
	if (!mesh) {
		return;
	}
	// 設定したデータをコマンドに積む
	const auto& commandList = DxCommand::GetCommandList();
	for (int i = 0; i < mesh->material_count(); ++i) {
		commandList->IASetVertexBuffers(0, 1, &mesh->get_vbv(i)); // VBV
		commandList->IASetIndexBuffer(mesh->get_p_ibv(i)); // IBV
		commandList->SetGraphicsRootConstantBufferView(0, transformMatrix->get_resource()->GetGPUVirtualAddress()); // Matrix
		commandList->SetGraphicsRootConstantBufferView(2, meshMaterials[i].buffer()->get_resource()->GetGPUVirtualAddress()); // Material
		commandList->SetGraphicsRootDescriptorTable(3, meshMaterials[i].texture->get_gpu_handle());
		commandList->DrawIndexedInstanced(mesh->index_size(i), 1, 0, 0, 0); // 描画コマンド
	}
}

void StaticMeshInstance::reset_mesh(const std::string& meshName_) {
	// メッシュ情報の取得
	mesh = PolygonMeshLibrary::GetPolygonMesh(meshName_);
#ifdef _DEBUG
	meshName = meshName_;
#endif // _DEBUG

	default_material();
}

void StaticMeshInstance::default_material() {
	meshMaterials.resize(mesh->material_count());

	for (int i = 0; auto & meshMaterial : meshMaterials) {
		// 色情報のリセット
		const auto* meshMaterialData = mesh->material_data(i);
		if (meshMaterialData) {
			// テクスチャ情報の取得
			meshMaterial.texture = TextureLibrary::GetTexture(meshMaterialData->textureFileName);
			// uv情報のリセット
			meshMaterial.uvTransform.copy(meshMaterialData->defaultUV);

#ifdef _DEBUG
			meshMaterial.textureName = meshMaterialData->textureFileName;
#endif // _DEBUG
		}
		else {
			meshMaterial.texture = TextureLibrary::GetTexture("Error.png");
			meshMaterial.uvTransform.copy(Transform2D{});
#ifdef _DEBUG
			meshMaterial.textureName = "Error.png";
#endif // _DEBUG
			Console("Warning : Material data is not found.\n");
		}
		++i;
	}
}

std::vector<StaticMeshInstance::PolygonMeshMaterial>& StaticMeshInstance::get_materials() {
	return meshMaterials;
}

void StaticMeshInstance::set_texture(const std::string& name, int index) {
	meshMaterials[index].texture = TextureLibrary::GetTexture(name);
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
	for (int i = 0; auto & meshMaterial : meshMaterials) {
		std::string treeNodeName;
		auto meshData = mesh->mesh_data(i);
		if (meshData) {
			treeNodeName = meshData->materialName + "##" + std::to_string(i);
		}
		if (treeNodeName.empty()) {
			treeNodeName = "UnknownMaterialName##" + std::to_string(i);
		}
		if (ImGui::TreeNodeEx(treeNodeName.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
			if (TextureLibrary::TextureListGui(meshMaterial.textureName)) {
				set_texture(meshMaterial.textureName, i);
				//meshMaterials[i].texture = TextureLibrary::GetTexture(meshMaterials[i].textureName);
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

StaticMeshInstance::PolygonMeshMaterial::PolygonMeshMaterial() :
	materialBuffer(eps::CreateUnique<Material>()) {
	lightingType = LighingType::HalfLambert;
	shininess = 50;
}

void StaticMeshInstance::PolygonMeshMaterial::write_buffer() {
	*materialBuffer->get_data() = {
		color,
		static_cast<uint32_t>(lightingType),
		shininess,
		{0,0,0},
		uvTransform.get_matrix4x4_padding()
	};
}

Reference<const Material> StaticMeshInstance::PolygonMeshMaterial::buffer() const {
	return materialBuffer;
}
