#include "ParticleSystemBillboard.h"

#include "Engine/Resources/Texture/TextureManager.h"
#include "Engine/Rendering/DirectX/DirectXResourceObject/Texture/Texture.h"
#include "Engine/Rendering/DirectX/DirectXCommand/DirectXCommand.h"

void ParticleSystemBillboard::draw() const {
	auto& commandList = DirectXCommand::GetCommandList();
	std::shared_ptr<Texture> lockedTexture = texture.lock();
	if (lockedTexture) {
		commandList->IASetVertexBuffers(0, 1, vertexBuffer.get_p_vbv());
		commandList->SetGraphicsRootDescriptorTable(0, particleBuffer.get_handle_gpu());
		commandList->SetGraphicsRootDescriptorTable(2, texture.lock()->get_gpu_handle());

		commandList->DrawInstanced(6, static_cast<UINT>(get_particles().size()), 0, 0);
	}
}

void ParticleSystemBillboard::create_rect(const Vector2& size, const Vector2& pivot) {
	auto lockedTexture = texture.lock();
	std::vector<BillboardBuffer> vertexData(6);
	vertexData[0] = {
		Vector2::Multiply(size, {-pivot.x, 1 - pivot.y}),
		CVector2::ZERO
	};
	vertexData[1] = {
		Vector2::Multiply(size, {-pivot.x, -pivot.y}),
		CVector2::BASIS_Y
	};
	vertexData[2] = {
		Vector2::Multiply(size, {1 - pivot.x, 1 - pivot.y}),
		CVector2::BASIS_X
	};
	vertexData[3] = {
		Vector2::Multiply(size, {-pivot.x, -pivot.y}),
		CVector2::BASIS_Y
	};
	vertexData[4] = {
		Vector2::Multiply(size, {1 - pivot.x, -pivot.y}),
		CVector2::BASIS
	};
	vertexData[5] = {
		Vector2::Multiply(size, {1 - pivot.x, 1 - pivot.y}),
		CVector2::BASIS_X
	};
	vertexBuffer.write(vertexData);
}

void ParticleSystemBillboard::set_texture(const std::string& textureName) {
	texture = TextureManager::GetTexture(textureName);
}
