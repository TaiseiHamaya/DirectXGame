#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "../IRemoteInstance.h"

#include "Engine/Module/World/WorldInstance/WorldInstance.h"

class RemoteCamera3dInstance final : public IRemoteInstance<WorldInstance> {
public:
	friend class EditorSceneSerializer;

public:
	RemoteCamera3dInstance() = default;
	~RemoteCamera3dInstance() = default;

	__CLASS_DEFAULT_ALL(RemoteCamera3dInstance)

public:
	void draw_inspector() override;

	nlohmann::json serialize() const override;

private:
	EditorValueField<r32> fovY{ "FOV Y" };
	EditorValueField<r32> aspectRatio{ "AspectRatio" };
	EditorValueField<r32> nearClip{ "NearClip" };
	EditorValueField<r32> farClip{ "FarClip" };
};

#endif // DEBUG_FEATURES_ENABLE
