#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Engine/Module/Transform2D/Transform2D.h"
#include "Engine/Module/WorldInstance/WorldInstance.h"

class Material;
class PolygonMesh;
class Texture;
class TransformMatrix;
class Color;

class GameObject : public WorldInstance {
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
	virtual void begin() {};
	virtual void update() {};
	virtual void begin_rendering() noexcept;
	virtual void late_update() {};
	virtual void draw() const;

	void reset_object(const std::string& meshName_);

	/// <summary>
	/// Texture、Materialパラメータ、UVデータのリセットを行う
	/// </summary>
	void default_material();

protected:
	struct MaterialDataRef;

public:
	std::vector<MaterialDataRef>& get_materials();

protected:
	void set_texture(const std::string& name, int index = 0);

#ifdef _DEBUG
public:
	void debug_gui() override;
#endif // _DEBUG

private:
	bool isDraw = true;
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
	struct MaterialDataRef {
		MaterialDataRef(Color& color_, Transform2D& uvTransform_);
		Color& color;
		Transform2D& uvTransform;
	};

	std::vector<MaterialDataRef> materialData;
};
