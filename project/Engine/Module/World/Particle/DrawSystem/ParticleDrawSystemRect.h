#pragma once

#include "BaseParticleDrawSystem.h"

#include <memory>
#include <string>

#include <Library/Math/Vector2.h>

#include "Engine/GraphicsAPI/DirectX/DxResource/Texture/Texture.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/VertexBuffer/VertexBuffer.h"

class Camera3D;

class ParticleDrawSystemRect final : public BaseParticleDrawSystem {
public:
	ParticleDrawSystemRect(const std::string& textureName);
	~ParticleDrawSystemRect() = default;

public:
	struct BillboardBuffer {
		Vector2 vertex;
		Vector2 texcoord;
	};

public:
	void draw_command(size_t InstanceCount) const override;

public:
	void create_rect(const Vector2& size, const Vector2& pivot = CVector2::ZERO);
	void set_texture(const std::string& textureName);

private:
	std::shared_ptr<const Texture> texture;

public:
	VertexBuffer<BillboardBuffer> vertexBuffer;
};
