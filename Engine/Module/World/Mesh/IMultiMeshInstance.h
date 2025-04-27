#pragma once

#include <memory>
#include <string>
#include <vector>

#include <Library/Math/Color3.h>
#include <Library/Math/Transform2D.h>

#include "./IDrawInstance.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/BufferObjects.h"

class PolygonMesh;
class Texture;

class IMultiMeshInstance : public IDrawInstance<std::string> {
public:
	struct Material {
		std::shared_ptr<const Texture> texture;
		Color3 color;
		Transform2D uvTransform;
		LighingType lightingType{ LighingType::HalfLambert };
		r32 shininess{ 50 };
	};

public:
	IMultiMeshInstance() noexcept;
	virtual ~IMultiMeshInstance() noexcept;

	__CLASS_NON_COPYABLE(IMultiMeshInstance)
	
public:
	std::vector<Material>& get_materials();
	const std::vector<Material>& get_materials() const;

protected:
	void set_texture(const std::string& name, i32 index = 0);

protected:
	std::vector<Material> materials;

#ifdef DEBUG_FEATURES_ENABLE
protected:
	std::shared_ptr<const PolygonMesh> mesh;
#endif // _DEBUG
};
