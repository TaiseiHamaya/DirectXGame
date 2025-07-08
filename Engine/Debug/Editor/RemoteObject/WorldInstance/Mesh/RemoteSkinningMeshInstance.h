#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "../IRemoteInstance.h"

#include <string>
#include <vector>

#include <Library/Math/Transform2D.h>

#include "Engine/GraphicsAPI/DirectX/DxResource/BufferObjects.h"

#define COLOR3_SERIALIZER
#include "Engine/Assets/Json/JsonSerializer.h"

class SkinningMeshInstance;

class SkeletonAsset;

class RemoteSkinningMeshInstance final : public IRemoteInstance<SkinningMeshInstance> {
public:
	friend class EditorSceneSerializer;

public:
	struct Material {
		std::string texture;
		Color3 color;
		Transform2D uvTransform;
		LighingType lightingType{ LighingType::HalfLambert };
		r32 shininess;
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
	EditorValueField<bool> isDraw{ "IsDraw" };
	EditorValueField<u32> layer{ "Layer" };

	std::string meshName;
	std::vector<Material> materials;

	std::string animationName;
	EditorValueField<bool> isLoop{ "IsLoop" };

	std::shared_ptr<const SkeletonAsset> skeleton;
};

#endif // DEBUG_FEATURES_ENABLE
