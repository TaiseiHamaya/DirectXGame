#pragma once

#include <memory>

class Texture;
class Transform2D;
class Material;
class TransformMatrix;
class Color;
class VertexBuffer;
class IndexBuffer;

class SpriteObject {
public:
	SpriteObject();
	explicit SpriteObject(const std::weak_ptr<Texture>& texture_);
	~SpriteObject();

private:
	SpriteObject(const SpriteObject&) = delete;
	SpriteObject operator=(const SpriteObject&) = delete;

public:
	const Transform2D& get_transform();
	//void update();
	void begin_rendering();
	void draw() const;

#ifdef _DEBUG
	void debug_gui();
#endif // _DEBUG

private:
	std::unique_ptr<VertexBuffer> vertices;
	std::unique_ptr<IndexBuffer> indexes;
	std::weak_ptr<Texture> texture;
	std::unique_ptr<Material> material;
	std::unique_ptr<TransformMatrix> transformMatrix;

	Color* const color;

	std::unique_ptr<Transform2D> transform;
	std::unique_ptr<Transform2D> uvTransform;
};
