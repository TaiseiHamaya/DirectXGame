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
class StaticMeshInstance;

class SkeletonAsset;

class RemoteSkinningMeshInstance final : public IRemoteInstance<SkinningMeshInstance, StaticMeshInstance> {
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
	RemoteSkinningMeshInstance();
	~RemoteSkinningMeshInstance() = default;

public:
	void setup() override;

	void update_preview(Reference<RemoteWorldObject> world, Reference<Affine> parentAffine) override;

	void draw_inspector() override;

	nlohmann::json serialize() const override;

	void on_spawn() override;

	void on_destroy() override;

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
