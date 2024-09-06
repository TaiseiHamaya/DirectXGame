#include "GameObject.h"

#include <format>

#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/DirectX/DirectXResourceObject/ConstantBuffer/TransformMatrix/TransformMatrix.h"
#include "Engine/DirectX/DirectXResourceObject/ConstantBuffer/Material/Material.h"
#include "Engine/DirectX/DirectXResourceObject/Texture/Texture.h"
#include "Engine/Game/Managers/TextureManager/TextureManager.h"
#include "Engine/Game/PolygonMesh/PolygonMesh.h"
#include "Engine/Game/Managers/PolygonMeshManager/PolygonMeshManager.h"
#include "Engine/Game/Transform3D/Transform3D.h"
#include "Engine/Game/Hierarchy/Hierarchy.h"
#include "Engine/Game/Camera/Camera3D.h"
#include "Engine/Utility/Utility.h"

#ifdef _DEBUG
#include "externals/imgui/imgui.h"
#endif // _DEBUG

GameObject::GameObject() noexcept(false) :
	// 各メモリの取得
	transformMatrix(std::make_unique<TransformMatrix>()),
	transform(std::make_unique<Transform3D>()),
	hierarchy(CreateUnique<Hierarchy>()){
	meshMaterials.clear();
	hierarchy->initialize(*transformMatrix);
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

void GameObject::begin_rendering(const Camera3D& camera) noexcept {

	Matrix4x4 worldMatrix = transform->get_matrix();
	if (hierarchy->has_parent()) {
		worldMatrix *= hierarchy->parent_matrix();
	}

	// 各情報をGPUに転送
	transformMatrix->set_transformation_matrix_data(
		std::move(worldMatrix),
		camera.vp_matrix()
	);
	for (int i = 0; i < meshMaterials.size(); ++i) {
		meshMaterials[i].material->set_uv_transform(meshMaterials[i].uvTransform.get_matrix4x4_transform());
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
		commandList->SetGraphicsRootConstantBufferView(1, meshMaterials[i].material->get_resource()->GetGPUVirtualAddress()); // Color
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
	materialData.resize(meshSize);

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
		materialData[i].color = &meshMaterials[i].color;
		materialData[i].uvTransform = &meshMaterials[i].uvTransform;
	}
}

const Matrix4x4& GameObject::world_matrix() const {
	return transformMatrix->get_data()->world;
}

const Vector3 GameObject::world_position() const {
	return Transform3D::ExtractPosition(world_matrix());
}

const Hierarchy& GameObject::get_hierarchy() const {
	return *hierarchy;
}

void GameObject::set_parent(const GameObject& object) {
	hierarchy->set_parent(object);
}

#ifdef _DEBUG
void GameObject::debug_gui() {
	if (PolygonMeshManager::MeshListGui(meshName)) {
		reset_object(meshName);
	}
	if (ImGui::Button("ResetMaterialData")) {
		default_material();
	}
	ImGui::Separator();
	transform->debug_gui();
	ImGui::Separator();
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
	material(CreateUnique<Material>()),
	color(material->get_color_reference()) {
	material->get_data()->color = Color{ 1.0f, 1.0f, 1.0f, 1.0f };
	material->get_data()->lighting = static_cast<std::uint32_t>(LighingType::HalfLambert);
	material->get_data()->padding = std::array<std::int32_t, 3>();
	material->get_data()->uvTransform = CMatrix4x4::IDENTITY;
}
