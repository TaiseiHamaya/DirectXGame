#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "../IRemoteInstance.h"

#include "Engine/Module/World/Camera/Camera3D.h"

class RemoteCamera3dInstance final : public IRemoteInstance<Camera3D, void*> {
public:
	friend class EditorSceneSerializer;

public:
	RemoteCamera3dInstance() = default;
	~RemoteCamera3dInstance() = default;

	__CLASS_DEFAULT_ALL(RemoteCamera3dInstance)

public:
	void update_preview(Reference<RemoteWorldObject> world, Reference<Affine> parentAffine) override;

	void draw_inspector() override;

	nlohmann::json serialize() const override;

private:
	EditorValueField<r32> fovY{ "FOV Y", 0.45f };
	EditorValueField<r32> aspectRatio{ "AspectRatio", 1.7777f };
	EditorValueField<r32> nearClip{ "NearClip", 0.1f };
	EditorValueField<r32> farClip{ "FarClip", 1000.0f };
};

#endif // DEBUG_FEATURES_ENABLE
