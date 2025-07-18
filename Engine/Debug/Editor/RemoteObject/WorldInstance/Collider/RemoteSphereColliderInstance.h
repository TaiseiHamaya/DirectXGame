#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "../IRemoteInstance.h"

class SphereCollider;

class RemoteSphereColliderInstance final : public IRemoteInstance<SphereCollider, void*> {
public:
	friend class EditorSceneSerializer;

public:
	RemoteSphereColliderInstance() = default;
	~RemoteSphereColliderInstance() = default;

	__CLASS_DEFAULT_ALL(RemoteSphereColliderInstance)

public:
	void update_preview(Reference<RemoteWorldObject> world, Reference<Affine> parentAffine) override;

	void draw_inspector() override;

	nlohmann::json serialize() const override;

private:
	EditorValueField<r32> radius{ "Radius", 1.0f };
};

#endif // DEBUG_FEATURES_ENABLE
