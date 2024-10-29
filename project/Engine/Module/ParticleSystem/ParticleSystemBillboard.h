#pragma once

#include "BaseParticleSystem.h"
#include "Engine/Rendering/DirectX/DirectXResourceObject/Texture/Texture.h"
#include "Library/Math/Vector2.h"
#include "Engine/Rendering/DirectX/DirectXResourceObject/VertexBuffer/VertexBuffer.h"

class Camera3D;

class ParticleSystemBillboard final : public BaseParticleSystem {
public:
	struct BillboardBuffer {
		Vector2 vertex;
		Vector2 texcoord;
	};

public:
	ParticleSystemBillboard() = default;
	~ParticleSystemBillboard() = default;

	ParticleSystemBillboard(const ParticleSystemBillboard&) = delete;
	ParticleSystemBillboard& operator=(const ParticleSystemBillboard&) = delete;
	ParticleSystemBillboard(ParticleSystemBillboard&&) = default;
	ParticleSystemBillboard& operator=(ParticleSystemBillboard&&) = default;

public:
	void draw() const;

public:
	void create_rect(const Vector2& size, const Vector2& pivot = CVector2::ZERO);
	void set_texture(const std::string& textureName);

private:
	std::weak_ptr<Texture> texture;

	Camera3D* lookatCamera;

public:
	VertexBuffer<BillboardBuffer> vertexBuffer;

private:
	inline static Camera3D* DefaultLookatCamera = nullptr;
};
