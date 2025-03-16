#pragma once

#include <memory>
#include <string>
#include <vector>

#include <Library/Math/Color3.h>
#include <Library/Math/Transform2D.h>

#include "Engine/Module/World/WorldInstance/WorldInstance.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/BufferObjects.h"

class Material;
class PolygonMesh;
class Texture;
class TransformMatrix;

class StaticMeshInstance : public WorldInstance {
	friend class SkinningMeshInstance;
public:
	struct Material {
		std::shared_ptr<const Texture> texture;
		Color3 color;
		Transform2D uvTransform;
		LighingType lightingType{ LighingType::HalfLambert };
		float shininess{ 50 };
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
	void reset_mesh(const std::string& meshName_);

	/// <summary>
	/// Texture、Materialパラメータ、UVデータのリセットを行う
	/// </summary>
	void default_material();


	std::vector<Material>& get_materials();
	const std::vector<Material>& get_materials() const;
	const uint32_t& layer() const { return renderLayer; };
	const std::string& mesh_id() const { return meshName; };
	virtual bool is_draw() const;
	void set_draw(bool flag) { isDraw = flag; }

protected:
	void set_texture(const std::string& name, int index = 0);

#ifdef DEBUG_FEATURES_ENABLE
public:
	void debug_gui() override;
#endif // _DEBUG

protected:
	bool isDraw = true;

private:
	std::vector<Material> materials;

	// Feature(RenderLayer)
	uint32_t renderLayer{ 0 };

	// HOTFIX : stringだとメモリ量が多いので、識別ID形式にする
	std::string meshName;

#ifdef DEBUG_FEATURES_ENABLE
private:
	std::shared_ptr<const PolygonMesh> mesh;
#endif // _DEBUG
};
