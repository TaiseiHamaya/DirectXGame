#include "SpriteInstance.h"

#include <Library/Math/Transform2D.h>
#include <Library/Math/VectorConverter.h>

#include "Engine/Assets/Texture/TextureAsset.h"
#include "Engine/Assets/Texture/TextureLibrary.h"
#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/IndexBuffer/IndexBuffer.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/VertexBuffer/VertexBuffer.h"
#include "Engine/Module/World/Camera/Camera2D.h"

#ifdef DEBUG_FEATURES_ENABLE
#include <imgui.h>
#endif // _DEBUG

SpriteInstance::SpriteInstance() :
	material(std::make_unique<ConstantBuffer<SpriteMaterial>>(SpriteMaterial{ Color4{ 1.0f,1.0f,1.0f,1.0f }, CMatrix4x4::IDENTITY })),
	color(material->data_mut()->color),
	transformMatrix(std::make_unique<ConstantBuffer<Matrix4x4>>()),
	transform(std::make_unique<Transform2D>()),
	uvTransform(std::make_unique<Transform2D>()) {
}

SpriteInstance::SpriteInstance(const std::string& textureName, const Vector2& pivot) :
	SpriteInstance() {

	texture = TextureLibrary::GetTexture(textureName);
	create_local_vertices(pivot);
	std::vector<u32> indexData{ 0,1,2,1,3,2 };
	indexes = std::make_unique<IndexBuffer>(indexData);
}

SpriteInstance::~SpriteInstance() noexcept = default;

SpriteInstance::SpriteInstance(SpriteInstance&&) noexcept = default;

SpriteInstance& SpriteInstance::operator=(SpriteInstance&&) noexcept = default;

const Transform2D& SpriteInstance::get_transform() noexcept {
	return *transform;
}

void SpriteInstance::transfer() noexcept {
	*transformMatrix->data_mut() = transform->get_matrix4x4_transform() * Camera2D::GetVPMatrix();
	material->data_mut()->uvTransform = uvTransform->get_matrix4x4_transform();
}

void SpriteInstance::draw() const {
	const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList = DxCommand::GetCommandList();
	// 設定したデータをコマンドに積む
	commandList->IASetVertexBuffers(0, 1, &vertices->get_vbv()); // VBV
	commandList->IASetIndexBuffer(indexes->get_p_ibv());
	commandList->SetGraphicsRootConstantBufferView(0, transformMatrix->get_resource()->GetGPUVirtualAddress()); // Matrix
	commandList->SetGraphicsRootConstantBufferView(1, material->get_resource()->GetGPUVirtualAddress()); // Color,UV
	commandList->SetGraphicsRootDescriptorTable(2, texture->handle()); // Texture
	commandList->DrawIndexedInstanced(indexes->index_size(), 1, 0, 0, 0); // 描画コマンド
}

#ifdef DEBUG_FEATURES_ENABLE
void SpriteInstance::debug_gui() {
	transform->debug_gui();
	ImGui::Separator();
	uvTransform->debug_gui();
	ImGui::Separator();
	color.debug_gui();
}
#endif // _DEBUG

void SpriteInstance::create_local_vertices(const Vector2& pivot) {
	Vector2 base = { static_cast<r32>(1), static_cast<r32>(1) };
	std::vector<VertexDataBuffer> vertexData(4);
	vertexData[0] = {
		Converter::ToVector3(Vector2::Multiply(base, {-pivot.x, 1 - pivot.y}), 0),
		CVector2::ZERO
	};
	vertexData[1] = {
		Converter::ToVector3(Vector2::Multiply(base, {-pivot.x, -pivot.y}), 0),
		CVector2::BASIS_Y
	};
	vertexData[2] = {
		Converter::ToVector3(Vector2::Multiply(base, {1 - pivot.x, 1 - pivot.y}), 0),
		CVector2::BASIS_X
	};
	vertexData[3] = {
		Converter::ToVector3(Vector2::Multiply(base, {1 - pivot.x, -pivot.y}), 0),
		CVector2::BASIS
	};

	vertices = std::make_unique<Object3DVertexBuffer>(vertexData);
}
