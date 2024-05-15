#include "GameObject.h"

#include "Engine/DirectX/DirectXDevice/DirectXDevice.h"
#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"

#include "Engine/Math/Camera3D.h"
#include "Engine/GameObject/Transform2D/Transform2D.h"
#include "Engine/GameObject/Transform3D/Transform3D.h"
#include "Engine/GameObject/PolygonMesh/PolygonMesh.h"
#include "Engine/DirectX/DirectXResourceObject/VertexBuffer/VertexBuffer.h"
#include "Engine/DirectX/DirectXResourceObject/ConstantBuffer/Material/Material.h"
#include "Engine/DirectX/DirectXResourceObject/ConstantBuffer/TransformMatrix/TransformMatrix.h"

GameObject::GameObject() :
	material(std::make_unique<Material>(MaterialData{ Color{ 1.0f,1.0f,1.0f,1.0f }, true, {0,0,0}, Matrix4x4::identity })),
	color(material->get_color_addr()),
	transformMatrix(std::make_unique<TransformMatrix>()),
	transform(std::make_unique<Transform3D>()),
	uvTransform(std::make_unique<Transform2D>()) {
}

GameObject::GameObject(const std::weak_ptr<PolygonMesh>& mesh_) :
	GameObject() {
	mesh = mesh_;
}

GameObject::~GameObject() = default;

const Transform3D& GameObject::get_transform() {
	return *transform;
}

void GameObject::begin_rendering() {
	transformMatrix->set_transformation_matrix_data(
		transform->get_matrix(),
		transform->get_matrix() * Camera3D::GetVPMatrix()
	);
	material->set_uv_transform(uvTransform->get_matrix4x4());
}

void GameObject::draw() const {
	const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList = DirectXCommand::GetCommandList();
	// 設定したデータをコマンドに積む
	auto&& mesh_locked = mesh.lock();
	commandList->IASetVertexBuffers(0, 1, mesh_locked->get_p_vbv()); // VBV
	commandList->IASetIndexBuffer(mesh_locked->get_p_ibv());
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList->SetGraphicsRootConstantBufferView(0, transformMatrix->get_resource()->GetGPUVirtualAddress()); // Matrix
	commandList->SetGraphicsRootConstantBufferView(1, material->get_resource()->GetGPUVirtualAddress()); // Color
	//commandList->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU); // Texture
	//commandList->SetGraphicsRootDescriptorTable(2, mesh_locked->get_texture().get_handle_cpu()); // Texture
	commandList->DrawIndexedInstanced(mesh_locked->get_index_size(), 1, 0, 0, 0); // 描画コマンド
}

#ifdef _DEBUG

void GameObject::debug_gui() {
	transform->debug_gui();
	uvTransform->debug_gui();
	color->debug_gui();
}

#endif // _DEBUG