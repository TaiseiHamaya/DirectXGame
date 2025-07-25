#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "../IRemoteInstance.h"

#include <string>
#include <vector>

#include <Library/Math/Color3.h>
#include <Library/Math/Transform2D.h>

#include "Engine/GraphicsAPI/DirectX/DxResource/BufferObjects.h"

#define COLOR3_SERIALIZER
#include "Engine/Assets/Json/JsonSerializer.h"

class StaticMeshInstance;

class RemoteStaticMeshInstance final : public IRemoteInstance<StaticMeshInstance, StaticMeshInstance> {
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
	RemoteStaticMeshInstance();
	~RemoteStaticMeshInstance() = default;

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
};

#endif // DEBUG_FEATURES_ENABLE
