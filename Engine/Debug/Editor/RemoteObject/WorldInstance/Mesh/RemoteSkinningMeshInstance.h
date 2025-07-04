#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "../IRemoteInstance.h"

#include <memory>
#include <string>
#include <vector>

#include <Library/Math/Color3.h>
#include <Library/Math/Transform2D.h>

#include "Engine/GraphicsAPI/DirectX/DxResource/BufferObjects.h"

class SkinningMeshInstance;

class TextureAsset;

class RemoteSkinningMeshInstance final : public IRemoteInstance<SkinningMeshInstance> {
public:
	friend class EditorSceneSerializer;

public:
	struct Material {
		std::shared_ptr<const TextureAsset> texture;
		Color3 color;
		Transform2D uvTransform;
		LighingType lightingType{ LighingType::HalfLambert };
		r32 shininess{ 50 };
	};

	struct SkeletonSpaceInstance {
		Transform3D transform;
		Affine affine;
	};

public:
	RemoteSkinningMeshInstance() = default;
	~RemoteSkinningMeshInstance() = default;

public:
	void draw_inspector() override;

	nlohmann::json serialize() const override;

private:
	void default_material();

private:
	bool isDraw;
	u32 layer;

	std::string meshName;
	std::vector<Material> materials;

	std::string animationName;
	bool isLoop;

};

#endif // DEBUG_FEATURES_ENABLE
