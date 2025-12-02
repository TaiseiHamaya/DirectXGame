#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "../IRemoteInstance.h"

#include "Engine/Module/World/Light/PointLight/PointLightInstance.h"
#include "Engine/Module/World/Mesh/Primitive/Rect3d.h"

class RemotePointLightInstane final : public IRemoteInstance<PointLightInstance, Rect3d> {
public:
	friend class EditorSceneSerializer;

public:
	RemotePointLightInstane() = default;
	~RemotePointLightInstane() = default;

	__CLASS_DEFAULT_ALL(RemotePointLightInstane)

public:
	void setup() override;

	void update_preview(Reference<RemoteWorldObject> world, Reference<Affine> parentAffine) override;

	void draw_inspector() override;

	nlohmann::json serialize() const override;

	constexpr InstanceType instance_type() const { return InstanceType::PointLightInstance; }

private:
	EditorValueField<Color3> color{ "Color", CColor3::WHITE };
	EditorValueField<r32> intensity{ "Intensity", 1.0f };
	EditorValueField<r32> radius{ "Radius", 1.0f };
	EditorValueField<r32> decay{ "Decay", 0.0f };
	EditorValueField<u32> influenceLayer{ "Influence Layer", 1u };
};

#endif // DEBUG_FEATURES_ENABLE
