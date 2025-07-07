#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "../IRemoteInstance.h"

#include <string>
#include <deque>

#include <Library/Math/Transform2D.h>

#include "Engine/GraphicsAPI/DirectX/DxResource/BufferObjects.h"

#define COLOR3_SERIALIZER
#include "Engine/Assets/Json/JsonSerializer.h"

class SkinningMeshInstance;

class RemoteSkinningMeshInstance final : public IRemoteInstance<SkinningMeshInstance> {
public:
	friend class EditorSceneSerializer;

public:
	struct Material {
		std::string texture;
		EditorValueField<Color3> color{ "Color" };
		EditorValueField<Transform2D> uvTransform{ "UV Transform" };
		LighingType lightingType{ LighingType::HalfLambert };
		EditorValueField<r32> shininess{ "Shininess" };
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
	std::deque<Material> materials;

	std::string animationName;
	EditorValueField<bool> isLoop{ "IsLoop" };
};

#endif // DEBUG_FEATURES_ENABLE
