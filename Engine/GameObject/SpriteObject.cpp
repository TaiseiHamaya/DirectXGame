#include "SpriteObject.h"

#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"

#include "Engine/DirectX/DirectXResourceObject/ConstantBuffer/Material/Material.h"
#include "Engine/DirectX/DirectXResourceObject/ConstantBuffer/TransformMatrix/TransformMatrix.h"
#include "Engine/DirectX/DirectXResourceObject/IndexBuffer/IndexBuffer.h"
#include "Engine/DirectX/DirectXResourceObject/Texture/Texture.h"
#include "Engine/DirectX/DirectXResourceObject/VertexBuffer/VertexBuffer.h"
#include "Engine/GameObject/Transform2D/Transform2D.h"
#include "Engine/Math/Camera2D.h"

SpriteObject::SpriteObject() :
	material(std::make_unique<Material>(MaterialData{ Color{ 1.0f,1.0f,1.0f,1.0f }, false, {0,0,0}, Matrix4x4::identity })),
	color(material->get_color_addr()),
	transformMatrix(std::make_unique<TransformMatrix>()),
	transform(std::make_unique<Transform2D>()),
	uvTransform(std::make_unique<Transform2D>()) {
}

SpriteObject::SpriteObject(const std::weak_ptr<Texture>& texture_) :
	SpriteObject() {

	texture = texture_;
	auto&& tex = texture.lock();
	std::vector<VertexData> vertexData(4);
	vertexData[0] = {
		VertexData::Vector4{ {0,static_cast<float>(tex->get_texture_height()),0}, 1 },
		{0,1}, 
		Vec3::kBasisZ
	};
	vertexData[1] = {
		VertexData::Vector4{ {0,0,0}, 1 },
		Vector2{0,0}, 
		Vec3::kBasisZ 
	};
	vertexData[2] = {
		VertexData::Vector4{ {static_cast<float>(tex->get_texture_width()),static_cast<float>(tex->get_texture_height()),0}, 1},
		Vector2{1,1}, 
		Vec3::kBasisZ 
	};
	vertexData[3] = {
		VertexData::Vector4{ {static_cast<float>(tex->get_texture_width()),0,0}, 1 },
		Vector2{1,0},
		Vec3::kBasisZ 
	};

	vertices = std::make_unique<VertexBuffer>(vertexData);
	std::vector<std::uint32_t> indexData{0,1,2,1,3,2};
	indexes = std::make_unique<IndexBuffer>(indexData);
}

SpriteObject::~SpriteObject() = default;

const Transform2D& SpriteObject::get_transform() {
	return *transform;
}

void SpriteObject::begin_rendering() {
	transformMatrix->set_transformation_matrix_data(
		transform->get_matrix4x4_transform(),
		transform->get_matrix4x4_transform() * Camera2D::GetVPMatrix()
	);
	material->set_uv_transform(uvTransform->get_matrix4x4_transform());
}

void SpriteObject::draw() const {
	const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList = DirectXCommand::GetCommandList();
	// 設定したデータをコマンドに積む
	auto&& texture_lockd = texture.lock();
	commandList->IASetVertexBuffers(0, 1, vertices->get_p_vbv()); // VBV
	commandList->IASetIndexBuffer(indexes->get_p_ibv());
	commandList->SetGraphicsRootConstantBufferView(0, transformMatrix->get_resource()->GetGPUVirtualAddress()); // Matrix
	commandList->SetGraphicsRootConstantBufferView(1, material->get_resource()->GetGPUVirtualAddress()); // Color
	texture_lockd->set_command();
	commandList->DrawIndexedInstanced(indexes->get_index_size(), 1, 0, 0, 0); // 描画コマンド

}

#ifdef _DEBUG
void SpriteObject::debug_gui() {
	transform->debug_gui(1.0f);
	uvTransform->debug_gui();
	color->debug_gui();
}
#endif // _DEBUG
