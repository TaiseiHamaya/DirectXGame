#pragma once

#include <memory>
#include <string>
#include <vector>

#include <Library/Math/Color3.h>
#include <Library/Math/Transform2D.h>

#include "Engine/Module/World/WorldInstance/WorldInstance.h"

class Material;
class PolygonMesh;
class Texture;
class TransformMatrix;
enum class LighingType;

class StaticMeshInstance : public WorldInstance {
	friend class SkinningMeshInstance;
public:
	struct PolygonMeshMaterial {
		PolygonMeshMaterial();
	public:
		void write_buffer();
		Reference<const Material> buffer() const;

	private:
		std::unique_ptr<Material> materialBuffer;

	public:
		std::shared_ptr<const Texture> texture;
		Color3 color;
		Transform2D uvTransform;
		LighingType lightingType;
		float shininess;

#ifdef _DEBUG
		std::string textureName;
#endif // _DEBUG
	};

public:
	StaticMeshInstance() noexcept(false);
	explicit StaticMeshInstance(const std::string& meshName_) noexcept(false);
	virtual ~StaticMeshInstance() noexcept;

	StaticMeshInstance(StaticMeshInstance&&) noexcept;
	StaticMeshInstance& operator=(StaticMeshInstance&&) noexcept;

private:
	StaticMeshInstance(const StaticMeshInstance&) = delete;
	StaticMeshInstance& operator=(const StaticMeshInstance&) = delete;

public:
	virtual void transfer() noexcept;
	virtual void draw() const;

	void reset_mesh(const std::string& meshName_);

	/// <summary>
	/// Texture、Materialパラメータ、UVデータのリセットを行う
	/// </summary>
	void default_material();

protected:
	struct MaterialDataRef;

public:
	std::vector<PolygonMeshMaterial>& get_materials();

protected:
	void set_texture(const std::string& name, int index = 0);

#ifdef _DEBUG
public:
	void debug_gui() override;
#endif // _DEBUG

protected:
	bool isDraw = true;

private:
	std::shared_ptr<const PolygonMesh> mesh;

	std::unique_ptr<TransformMatrix> transformMatrix;

	std::vector<PolygonMeshMaterial> meshMaterials;

#ifdef _DEBUG
private:
	std::string meshName;
#endif // _DEBUG
};
