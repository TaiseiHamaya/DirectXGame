#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Engine/DirectX/DirectXResourceObject/ConstantBuffer/Material/Material.h"
#include "Engine/Game/Transform2D/Transform2D.h"
#include "Engine/Game/Transform3D/Transform3D.h"

class PolygonMesh;
class Texture;
class TransformMatrix;
class Color;
class Camera3D;

class GameObject {
public:
	GameObject() noexcept(false);
	explicit GameObject(const std::string& meshName_) noexcept(false);
	~GameObject() noexcept;

	GameObject(GameObject&&) noexcept;
	GameObject& operator=(GameObject&&) noexcept;

private:
	GameObject(const GameObject&) = delete;
	GameObject& operator=(const GameObject&) = delete;

public:
	const Transform3D& get_transform() noexcept;
	void update();
	void begin_rendering(const Camera3D& camera) noexcept;
	void draw() const;

	void reset_object(const std::string& meshName_);

	/// <summary>
	/// Texutre、Materialパラメータ、UVデータのリセットそ行う
	/// </summary>
	void default_material();

#ifdef _DEBUG
	void debug_gui();
#endif // _DEBUG

private:
	std::string meshName;
	std::weak_ptr<PolygonMesh> mesh;
	std::unique_ptr<TransformMatrix> transformMatrix;

	struct PolygonMeshMaterial {
		PolygonMeshMaterial();
		std::weak_ptr<Texture> texture;

		Material material;
		Color& color;

		Transform2D uvTransform;

#ifdef _DEBUG
		std::string textureName;
#endif // _DEBUG
	};
	std::vector<PolygonMeshMaterial> meshMaterials;

protected:
	std::unique_ptr<Transform3D> transform;
};
