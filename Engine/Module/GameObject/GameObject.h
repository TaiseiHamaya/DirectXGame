#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Engine/Module/Transform2D/Transform2D.h"
#include "Engine/Module/Transform3D/Transform3D.h"

class Material;
class PolygonMesh;
class Texture;
class TransformMatrix;
class Color;
class Camera3D;
class Hierarchy;

class GameObject {
public:
	GameObject() noexcept(false);
	explicit GameObject(const std::string& meshName_) noexcept(false);
	virtual ~GameObject() noexcept;

	GameObject(GameObject&&) noexcept;
	GameObject& operator=(GameObject&&) noexcept;

private:
	GameObject(const GameObject&) = delete;
	GameObject& operator=(const GameObject&) = delete;

public:
	virtual void begin();
	virtual void update();
	virtual void begin_rendering(const Camera3D& camera) noexcept;
	virtual void late_update();
	virtual void draw() const;

	void reset_object(const std::string& meshName_);

	/// <summary>
	/// Texture、Materialパラメータ、UVデータのリセットを行う
	/// </summary>
	void default_material();

protected:
	struct MaterialDataRef;

public:
	const Matrix4x4& world_matrix() const;
	const Vector3 world_position() const;
	void look_at(const GameObject& rhs, const Vector3& upwards = CVector3::BASIS_Y) noexcept;
	void look_at(const Vector3& point, const Vector3& upwards = CVector3::BASIS_Y) noexcept;

	Transform3D& get_transform() noexcept;
	const Transform3D& get_transform() const noexcept;
	const Hierarchy& get_hierarchy() const;
	void set_parent(const Hierarchy& hierarchy);
	std::vector<MaterialDataRef>& get_materials();

protected:
	void set_texture(const std::string& name, int index = 0);

#ifdef _DEBUG
public:
	void debug_gui();
#endif // _DEBUG

private:
	std::string meshName;
	std::weak_ptr<PolygonMesh> mesh;
	std::unique_ptr<TransformMatrix> transformMatrix;

	struct PolygonMeshMaterial {
		PolygonMeshMaterial();
		std::weak_ptr<Texture> texture;

		std::unique_ptr<Material> material;
		Color& color;

		Transform2D uvTransform;

#ifdef _DEBUG
		std::string textureName;
#endif // _DEBUG
	};
	std::vector<PolygonMeshMaterial> meshMaterials;

protected:
	std::unique_ptr<Transform3D> transform;
	std::unique_ptr<Hierarchy> hierarchy;

	struct MaterialDataRef {
		MaterialDataRef(Color& color_, Transform2D& uvTransform_);
		Color& color;
		Transform2D& uvTransform;
	};

	std::vector<MaterialDataRef> materialData;
};
