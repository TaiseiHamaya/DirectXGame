#include "GameObject.h"

#include <format>

#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/DirectX/DirectXResourceObject/ConstantBuffer/TransformMatrix/TransformMatrix.h"
#include "Engine/DirectX/DirectXResourceObject/Texture/Texture.h"
#include "Engine/DirectX/DirectXResourceObject/Texture/TextureManager/TextureManager.h"
#include "Engine/GameObject/PolygonMesh/PolygonMesh.h"
#include "Engine/GameObject/PolygonMesh/PolygonMeshManager/PolygonMeshManager.h"
#include "Engine/GameObject/Transform3D/Transform3D.h"
#include "Engine/Math/Camera3D.h"
#include "Engine/Utility/Utility.h"

#ifdef _DEBUG
#include "externals/imgui/imgui.h"
#endif // _DEBUG

GameObject::GameObject() noexcept(false) :
	// 各メモリの取得
	transformMatrix(std::make_unique<TransformMatrix>()),
	transform(std::make_unique<Transform3D>()) {
	meshMaterials.clear();
}

GameObject::GameObject(const std::string& meshName_) noexcept(false) :
	GameObject() {
	reset_object(meshName_);
}

GameObject::~GameObject() noexcept = default;

GameObject::GameObject(GameObject&&) noexcept = default;

GameObject& GameObject::operator=(GameObject&&) noexcept = default;

const Transform3D& GameObject::get_transform() noexcept {
	return *transform;
}

void GameObject::update() {
}

void GameObject::begin_rendering() noexcept {
	// 各情報をGPUに転送
	transformMatrix->set_transformation_matrix_data(
		transform->get_matrix(),
		static_cast<Matrix4x4>(transform->get_matrix() * Camera3D::GetVPMatrix())
	);
	for (int i = 0; i < meshMaterials.size(); ++i) {
		meshMaterials[i].material.set_uv_transform(meshMaterials[i].uvTransform.get_matrix4x4_transform());
	}
}

void GameObject::draw() const {
	const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList = DirectXCommand::GetCommandList();
	// 設定したデータをコマンドに積む
	auto&& meshLocked = mesh.lock();
	for (int i = 0; i < meshLocked->material_count(); ++i) {
		commandList->IASetVertexBuffers(0, 1, meshLocked->get_p_vbv(i)); // VBV
		commandList->IASetIndexBuffer(meshLocked->get_p_ibv(i)); // IBV
		commandList->SetGraphicsRootConstantBufferView(0, transformMatrix->get_resource()->GetGPUVirtualAddress()); // Matrix
		commandList->SetGraphicsRootConstantBufferView(1, meshMaterials[i].material.get_resource()->GetGPUVirtualAddress()); // Color
		if (meshMaterials[i].texture.expired()) {
			// テクスチャ情報が存在しないならエラーテクスチャを使用
			TextureManager::GetTexture("Error.png").lock()->set_command();
		}
		else {
			meshMaterials[i].texture.lock()->set_command();
		}
		commandList->DrawIndexedInstanced(meshLocked->index_size(i), 1, 0, 0, 0); // 描画コマンド
	}
}

void GameObject::reset_object(const std::string& meshName_) {
	meshName = meshName_;
	mesh = PolygonMeshManager::GetPolygonMesh(meshName);
	size_t meshSize = mesh.lock()->material_count();

	meshMaterials.resize(meshSize);

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
			Log(std::format("[GameObject] Mtl file used Object file \'{}\' is not found", meshName));
		}
	}
}

#ifdef _DEBUG
void GameObject::debug_gui() {
	if (PolygonMeshManager::MeshListGui(meshName)) {
		reset_object(meshName);
	}
	if (ImGui::Button("ResetMaterialData")) {
		default_material();
	}
	transform->debug_gui();
	auto&& meshLocked = mesh.lock();
	for (int i = 0; i < meshMaterials.size(); ++i) {
		std::string treeNodeNmae = meshLocked->model_name(i).empty() ? "UnknownMaterialName" : meshLocked->model_name(i);
		if (ImGui::TreeNodeEx(treeNodeNmae.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
			meshMaterials[i].uvTransform.debug_gui();
			meshMaterials[i].color.debug_gui();
			if (TextureManager::TextureListGui(meshMaterials[i].textureName)) {
				meshMaterials[i].texture = TextureManager::GetTexture(meshMaterials[i].textureName);
			}
			ImGui::Text("Lighting option");
			if (ImGui::Button("None")) {
				meshMaterials[i].material.set_lighting(LighingType::None);
			}
			ImGui::SameLine();
			if (ImGui::Button("Lambert")) {
				meshMaterials[i].material.set_lighting(LighingType::Lambert);
			}
			ImGui::SameLine();
			if (ImGui::Button("Half lambert")) {
				meshMaterials[i].material.set_lighting(LighingType::HalfLambert);
			}
			ImGui::TreePop();
		}
	}
}
#endif // _DEBUG

GameObject::PolygonMeshMaterial::PolygonMeshMaterial() :
	material({ Color{1.0f, 1.0f, 1.0f, 1.0f}, static_cast<std::uint32_t>(LighingType::HalfLambert), std::array<std::int32_t, 3>(), CMatrix4x4::IDENTITY }),
	color(material.get_color_reference()) {
}
