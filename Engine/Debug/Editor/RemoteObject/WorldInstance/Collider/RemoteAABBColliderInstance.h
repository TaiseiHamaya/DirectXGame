#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "../IRemoteInstance.h"

class AABBCollider;

class RemoteAABBColliderInstance final : public IRemoteInstance<AABBCollider, void*> {
public:
	friend class EditorSceneSerializer;

public:
	RemoteAABBColliderInstance() = default;
	~RemoteAABBColliderInstance() = default;

	__CLASS_DEFAULT_ALL(RemoteAABBColliderInstance)

public:
	void draw_inspector() override;

	nlohmann::json serialize() const override;

	void set_editor_world_view(Reference<EditorWorldView> worldView, Reference<const Affine> parentAffine = nullptr) override;

private:
	EditorValueField<Vector3> size{ "Size", CVector3::ONE * 2 };
	EditorValueField<Vector3> offset{ "Offset", CVector3::ONE * 0.5f };
};

#endif // DEBUG_FEATURES_ENABLE
