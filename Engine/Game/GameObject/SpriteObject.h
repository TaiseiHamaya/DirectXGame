#pragma once

#include <memory>
#include <string>

#include "Engine/Math/Vector2.h"
#include "Engine/Math/Matrix4x4.h"
#include "Engine/Game/Color/Color.h"
#include "Engine/DirectX/DirectXResourceObject/ConstantBuffer/ConstantBuffer.h"

class Texture;
class Transform2D;
class VertexBuffer;
class IndexBuffer;

class SpriteObject {
public:
	SpriteObject() noexcept(false);
	explicit SpriteObject(const std::string& textureName, const Vector2& pivot) noexcept(false);
	~SpriteObject() noexcept;

	SpriteObject(SpriteObject&&) noexcept;
	SpriteObject& operator=(SpriteObject&&) noexcept;

private:
	SpriteObject(const SpriteObject&) = delete;
	SpriteObject operator=(const SpriteObject&) = delete;

public:
	const Transform2D& get_transform() noexcept;
	//void update();
	void begin_rendering() noexcept;
	void draw() const;

#ifdef _DEBUG
	void debug_gui();
#endif // _DEBUG

private:
	void create_local_vertices(const Vector2& pivot);

private:
	std::unique_ptr<VertexBuffer> vertices;
	std::unique_ptr<IndexBuffer> indexes;
	std::weak_ptr<Texture> texture;

	struct SpriteMaterial {
		Color color;
		Matrix4x4 uvTransform;
	};

	std::unique_ptr<ConstantBuffer<SpriteMaterial>> material;
	std::unique_ptr<ConstantBuffer<Matrix4x4>> transformMatrix;

protected:
	Color& color;

	std::unique_ptr<Transform2D> transform;
	std::unique_ptr<Transform2D> uvTransform;
};
