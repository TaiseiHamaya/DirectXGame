#pragma once

#include <memory>
#include <string>

class Transform2D;
class Transform3D;
class PolygonMesh;
class Texture;
class Material;
class TransformMatrix;
class Color;

class GameObject {
public:
	GameObject();
	explicit GameObject(const std::string& meshName_);
	~GameObject();

	GameObject(GameObject&&) noexcept;
	GameObject& operator=(GameObject&&) noexcept;
private:
	GameObject(const GameObject&) = delete;
	GameObject& operator=(const GameObject&) = delete;

public:
	const Transform3D& get_transform();
	//void update();
	void begin_rendering();
	void draw() const;

#ifdef _DEBUG
	void debug_gui();
#endif // _DEBUG

	void reset_default();

private:
	std::weak_ptr<PolygonMesh> mesh;
	std::weak_ptr<Texture> texture;

	std::unique_ptr<Material> material;
	std::unique_ptr<TransformMatrix> transformMatrix;

	Color* color;

	std::unique_ptr<Transform3D> transform;
	std::unique_ptr<Transform2D> uvTransform;

	std::string meshName;
	std::string textureName;
};
