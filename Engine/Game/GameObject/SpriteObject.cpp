#include "SpriteObject.h"

#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"

#include "Engine/DirectX/DirectXResourceObject/IndexBuffer/IndexBuffer.h"
#include "Engine/DirectX/DirectXResourceObject/Texture/Texture.h"
#include "Engine/Game/Managers/TextureManager/TextureManager.h"
#include "Engine/DirectX/DirectXResourceObject/VertexBuffer/VertexBuffer.h"
#include "Engine/Game/Transform2D/Transform2D.h"
#include "Engine/Game/Camera/Camera2D.h"

#ifdef _DEBUG
#include <externals/imgui/imgui.h>
#endif // _DEBUG

SpriteObject::SpriteObject() :
	material(std::make_unique<ConstantBuffer<SpriteMaterial>>(SpriteMaterial{ Color{ 1.0f,1.0f,1.0f,1.0f }, CMatrix4x4::IDENTITY })),
	color(material->get_data()->color),
	transformMatrix(std::make_unique<ConstantBuffer<Matrix4x4>>()),
	transform(std::make_unique<Transform2D>()),
	uvTransform(std::make_unique<Transform2D>()) {
}

SpriteObject::SpriteObject(const std::string& textureName, const Vector2& pivot) :
	SpriteObject() {

	texture = TextureManager::GetTexture(textureName);
	create_local_vertices(pivot);
	std::vector<std::uint32_t> indexData{ 0,1,2,1,3,2 };
	indexes = std::make_unique<IndexBuffer>(indexData);
}

SpriteObject::~SpriteObject() noexcept = default;

SpriteObject::SpriteObject(SpriteObject&&) noexcept = default;

SpriteObject& SpriteObject::operator=(SpriteObject&&) noexcept = default;

const Transform2D& SpriteObject::get_transform() noexcept {
	return *transform;
}

void SpriteObject::begin_rendering() noexcept {
	*transformMatrix->get_data() = transform->get_matrix4x4_transform() * Camera2D::GetVPMatrix();
	material->get_data()->uvTransform = uvTransform->get_matrix4x4_transform();
}

void SpriteObject::draw() const {
	const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList = DirectXCommand::GetCommandList();
	// 設定したデータをコマンドに積む
	auto&& texture_locked = texture.lock();
	commandList->IASetVertexBuffers(0, 1, vertices->get_p_vbv()); // VBV
	commandList->IASetIndexBuffer(indexes->get_p_ibv());
	commandList->SetGraphicsRootConstantBufferView(0, transformMatrix->get_resource()->GetGPUVirtualAddress()); // Matrix
	commandList->SetGraphicsRootConstantBufferView(1, material->get_resource()->GetGPUVirtualAddress()); // Color,UV
	texture_locked->set_command();
	commandList->DrawIndexedInstanced(indexes->index_size(), 1, 0, 0, 0); // 描画コマンド
}

#ifdef _DEBUG
void SpriteObject::debug_gui() {
	transform->debug_gui(1.0f);
	ImGui::Separator();
	uvTransform->debug_gui();
	ImGui::Separator();
	color.debug_gui4();
}
#endif // _DEBUG

void SpriteObject::create_local_vertices(const Vector2& pivot) {
	auto&& tex = texture.lock();
	Vector2 base = { static_cast<float>(tex->get_texture_width()), static_cast<float>(tex->get_texture_height()) };
	std::vector<VertexData> vertexData(4);
	vertexData[0] = {
		VertexData::Vector4{ Vector2::Multiply(base, {-pivot.x, 1 - pivot.y}).convert(0), 1},
		CVector2::BASIS_Y
	};
	vertexData[1] = {
		VertexData::Vector4{ Vector2::Multiply(base, {-pivot.x, -pivot.y}).convert(0), 1},
		CVector2::ZERO
	};
	vertexData[2] = {
		VertexData::Vector4{ Vector2::Multiply(base, {1 - pivot.x, 1 - pivot.y}).convert(0), 1},
		CVector2::BASIS
	};
	vertexData[3] = {
		VertexData::Vector4{ Vector2::Multiply(base, {1 - pivot.x, -pivot.y}).convert(0), 1},
		CVector2::BASIS_X
	};

	vertices = std::make_unique<VertexBuffer>(vertexData);
}
