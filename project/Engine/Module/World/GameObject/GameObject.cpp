#include "GameObject.h"

#include "Engine/Debug/Output.h"
#include "Engine/Rendering/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/Rendering/DirectX/DirectXResourceObject/ConstantBuffer/Material/Material.h"
#include "Engine/Rendering/DirectX/DirectXResourceObject/ConstantBuffer/TransformMatrix/TransformMatrix.h"
#include "Engine/Rendering/DirectX/DirectXResourceObject/Texture/Texture.h"
#include "Library/Math/Hierarchy.h"
#include "Engine/Resources/PolygonMesh/PolygonMesh.h"
#include "Engine/Resources/PolygonMesh/PolygonMeshManager.h"
#include "Engine/Resources/Texture/TextureManager.h"

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

GameObject::GameObject() noexcept(false) :
	// 各メモリの取得
	transformMatrix(std::make_unique<TransformMatrix>()) {
	meshMaterials.clear();
	hierarchy.initialize(*transformMatrix->get_data());
}

GameObject::GameObject(const std::string& meshName_) noexcept(false) :
	GameObject() {
	reset_object(meshName_);
}

GameObject::~GameObject() noexcept = default;

GameObject::GameObject(GameObject&&) noexcept = default;

GameObject& GameObject::operator=(GameObject&&) noexcept = default;

void GameObject::begin_rendering() noexcept {
	if (!isActive) {
		return;
	}
	// World行列更新
	update_matrix();
	// 各情報をGPUに転送
	// Transformに転送
	transformMatrix->set_transformation_matrix_data(world_matrix());
	// Materialに転送
	for (int i = 0; i < meshMaterials.size(); ++i) {
		meshMaterials[i].material->set_uv_transform(meshMaterials[i].uvTransform.get_matrix4x4_transform());
	}
}

void GameObject::draw() const {
	if (!isActive || !isDraw) {
		return;
	}
	const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList = DirectXCommand::GetCommandList();
	// 設定したデータをコマンドに積む
	auto&& meshLocked = mesh.lock();
	if (!meshLocked) {
		return;
	}
	for (int i = 0; i < meshLocked->material_count(); ++i) {
		commandList->IASetVertexBuffers(0, 1, meshLocked->get_p_vbv(i)); // VBV
		commandList->IASetIndexBuffer(meshLocked->get_p_ibv(i)); // IBV
		commandList->SetGraphicsRootConstantBufferView(0, transformMatrix->get_resource()->GetGPUVirtualAddress()); // Matrix
		commandList->SetGraphicsRootConstantBufferView(2, meshMaterials[i].material->get_resource()->GetGPUVirtualAddress()); // Color
		const auto& lockedTexture = meshMaterials[i].texture.lock();
		commandList->SetGraphicsRootDescriptorTable(4, 
			lockedTexture ? 
			lockedTexture->get_gpu_handle() : 
			TextureManager::GetTexture("Error.png").lock()->get_gpu_handle()
		);
		commandList->DrawIndexedInstanced(meshLocked->index_size(i), 1, 0, 0, 0); // 描画コマンド
	}
}

void GameObject::reset_object(const std::string& meshName_) {
	meshName = meshName_;
	mesh = PolygonMeshManager::GetPolygonMesh(meshName);
	size_t meshSize = mesh.lock()->material_count();

	meshMaterials.resize(meshSize);
	materialData.clear();

	default_material();
}

void GameObject::default_material() {
	auto&& meshLocked = mesh.lock();
	for (int i = 0; i < meshMaterials.size(); ++i) {
		// 色情報のリセット
		meshMaterials[i].color = Color{ 1.0f,1.0f,1.0f,1.0f };
		if (meshLocked->has_mtl(i)) {
			// テクスチャ情報の取得
			meshMaterials[i].texture = TextureManager::GetTexture(meshLocked->texture_name(i));
			// uv情報のリセット
			meshMaterials[i].uvTransform.copy(meshLocked->default_uv(i));

#ifdef _DEBUG
			meshMaterials[i].textureName = meshLocked->texture_name(i);
#endif // _DEBUG
		}
		else {
			meshMaterials[i].texture = TextureManager::GetTexture("Error.png");
			meshMaterials[i].uvTransform.copy(Transform2D{});
#ifdef _DEBUG
			meshMaterials[i].textureName = "Error.png";
#endif // _DEBUG
			Console("[GameObject] Mtl file used Object file \'{}\' is not found.\n", meshName);
		}
		materialData.emplace_back(meshMaterials[i].color, meshMaterials[i].uvTransform);
	}
}

std::vector<GameObject::MaterialDataRef>& GameObject::get_materials() {
	return materialData;
}

void GameObject::set_texture(const std::string& name, int index) {
	meshMaterials[index].texture = TextureManager::GetTexture(name);
}

#ifdef _DEBUG
void GameObject::debug_gui() {
	if (PolygonMeshManager::MeshListGui(meshName)) {
		reset_object(meshName);
	}
	if (ImGui::Button("ResetMaterialData")) {
		default_material();
	}
	ImGui::Checkbox("Draw", &isDraw);
	ImGui::Separator();
	WorldInstance::debug_gui();
	ImGui::Separator();
	ImGui::Text("Materials");
	auto&& meshLocked = mesh.lock();
	for (int i = 0; i < meshMaterials.size(); ++i) {
		std::string treeNodeName = meshLocked->model_name(i).empty() ? "UnknownMaterialName" : meshLocked->model_name(i);
		if (ImGui::TreeNodeEx(treeNodeName.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
			if (TextureManager::TextureListGui(meshMaterials[i].textureName)) {
				meshMaterials[i].texture = TextureManager::GetTexture(meshMaterials[i].textureName);
			}

			meshMaterials[i].uvTransform.debug_gui();

			meshMaterials[i].color.debug_gui3();

			if (ImGui::RadioButton("None", meshMaterials[i].material->get_data()->lighting == static_cast<uint32_t>(LighingType::None))) {
				meshMaterials[i].material->set_lighting(LighingType::None);
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("Lambert", meshMaterials[i].material->get_data()->lighting == static_cast<uint32_t>(LighingType::Lambert))) {
				meshMaterials[i].material->set_lighting(LighingType::Lambert);
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("Half lambert", meshMaterials[i].material->get_data()->lighting == static_cast<uint32_t>(LighingType::HalfLambert))) {
				meshMaterials[i].material->set_lighting(LighingType::HalfLambert);
			}
			ImGui::TreePop();
		}
	}
}
#endif // _DEBUG

GameObject::PolygonMeshMaterial::PolygonMeshMaterial() :
	material(std::make_unique<Material>()),
	color(material->get_color_reference()) {
	material->get_data()->color = Color{ 1.0f, 1.0f, 1.0f, 1.0f };
	material->get_data()->lighting = static_cast<std::uint32_t>(LighingType::HalfLambert);
	material->get_data()->padding = std::array<std::int32_t, 3>();
	material->get_data()->uvTransform = CMatrix4x4::IDENTITY;
}

GameObject::MaterialDataRef::MaterialDataRef(Color& color_, Transform2D& uvTransform_) :
	color(color_),
	uvTransform(uvTransform_) {
}
