#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "../IRemoteInstance.h"

#include "Engine/Module/World/Camera/Camera3D.h"

class SphereCollider;

class RemoteSphereColliderInstance final : public IRemoteInstance<SphereCollider, void*> {
public:
	friend class EditorSceneSerializer;

public:
	RemoteSphereColliderInstance() = default;
	~RemoteSphereColliderInstance() = default;

	__CLASS_DEFAULT_ALL(RemoteSphereColliderInstance)

public:
	void draw_inspector() override;

	nlohmann::json serialize() const override;

	void set_editor_world_view(Reference<EditorWorldView> worldView, Reference<const Affine> parentAffine = nullptr) override;

private:
	EditorValueField<r32> radius{ "Radius", 1.0f };
};

#endif // DEBUG_FEATURES_ENABLE
