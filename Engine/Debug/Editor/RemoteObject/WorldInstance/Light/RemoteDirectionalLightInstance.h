#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "../IRemoteInstance.h"

#include "Engine/Module/World/Light/DirectionalLight/DirectionalLightInstance.h"

class RemoteDirectionalLightInstance : public IRemoteInstance<DirectionalLightInstance> {
public:
	friend class EditorSceneSerializer;

public:
	RemoteDirectionalLightInstance() = default;
	~RemoteDirectionalLightInstance() = default;

	__CLASS_DEFAULT_ALL(RemoteDirectionalLightInstance)

public:
	void update_preview(Reference<RemoteWorldObject> world, Reference<Affine> parentAffine) override;

	void draw_inspector() override;

	nlohmann::json serialize() const override;

	constexpr InstanceType instance_type() const { return InstanceType::DirectionalLightInstance; }

private:
	EditorValueField<Color3> color{ "Color", CColor3::WHITE };
	EditorValueField<r32> intensity{ "Intensity", 1.0f };
	EditorValueField<Vector3> direction{ "Direction", CVector3::DOWN };

	EditorValueField<u32> influenceLayer{ "Influence Layer", 1 };
};

#endif // DEBUG_FEATURES_ENABLE
