#pragma once

#include "./IDrawInstance.h"

#include <memory>
#include <string>
#include <vector>

#include <Library/Math/Color3.h>
#include <Library/Math/Transform2D.h>

#include "Engine/GraphicsAPI/DirectX/DxResource/BufferObjects.h"

class PolygonMesh;
class TextureAsset;

class IMultiMeshInstance : public IDrawInstance<std::string> {
public:
	struct Material {
		std::shared_ptr<const TextureAsset> texture;
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
};
