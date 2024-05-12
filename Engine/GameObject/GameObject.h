#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <memory>

class Transform2D;
class Transform3D;
class PolygonMesh;
class Material;
class TransformMatrix;
class Color;

class GameObject {
public:
	GameObject();
	GameObject(const std::weak_ptr<PolygonMesh>& mesh_);
	~GameObject();

private:
	GameObject(const GameObject&) = delete;
	GameObject operator=(const GameObject&) = delete;

public:
	const Transform3D& get_transform();
	//void update();
	void begin_rendering();
	void draw() const;

private:
	std::weak_ptr<PolygonMesh> mesh;
	std::unique_ptr<Material> material;
	std::unique_ptr<TransformMatrix> transformMatrix;

	Color* const color;

	std::unique_ptr<Transform3D> transform;
	std::unique_ptr<Transform2D> uvTransform;
};
