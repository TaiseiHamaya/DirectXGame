#pragma once

#include <memory>
#include <string>

#include <Library/Math/Color4.h>
#include <Library/Math/Matrix4x4.h>
#include <Library/Math/Vector2.h>

#include "Engine/GraphicsAPI/DirectX/DxResource/ConstantBuffer/ConstantBuffer.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/VertexBuffer/VertexBuffer.h"

class Texture;
class Transform2D;
class IndexBuffer;

class SpriteInstance {
public:
	SpriteInstance() noexcept(false);
	explicit SpriteInstance(const std::string& textureName, const Vector2& pivot = CVector2::ZERO) noexcept(false);
	~SpriteInstance() noexcept;

	SpriteInstance(SpriteInstance&&) noexcept;
	SpriteInstance& operator=(SpriteInstance&&) noexcept;

private:
	SpriteInstance(const SpriteInstance&) = delete;
	SpriteInstance& operator=(const SpriteInstance&) = delete;

public:
	const Transform2D& get_transform() noexcept;
	//void update();
	void transfer() noexcept;
	void draw() const;

#ifdef _DEBUG
	void debug_gui();
#endif // _DEBUG

private:
	void create_local_vertices(const Vector2& pivot);

private:
	std::unique_ptr<Object3DVertexBuffer> vertices;
	std::unique_ptr<IndexBuffer> indexes;
	std::shared_ptr<const Texture> texture;

	struct SpriteMaterial {
		Color4 color;
		Matrix4x4 uvTransform;
	};

	std::unique_ptr<ConstantBuffer<SpriteMaterial>> material;
	std::unique_ptr<ConstantBuffer<Matrix4x4>> transformMatrix;

protected:
	Color4& color;

	std::unique_ptr<Transform2D> transform;
	std::unique_ptr<Transform2D> uvTransform;
};
