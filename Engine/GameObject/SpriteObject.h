#pragma once

#include <memory>
#include <string>
#include "Engine/Math/Vector2.h"

class Texture;
class Transform2D;
class Material;
class TransformMatrix;
class Color;
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
	std::unique_ptr<Material> material;
	std::unique_ptr<TransformMatrix> transformMatrix;

protected:
	Color& color;

	std::unique_ptr<Transform2D> transform;
	std::unique_ptr<Transform2D> uvTransform;
};
