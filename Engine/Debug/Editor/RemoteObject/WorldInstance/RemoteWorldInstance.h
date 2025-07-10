#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "./IRemoteInstance.h"

#include "Engine/Module/World/WorldInstance/WorldInstance.h"

class RemoteWorldInstance final : public IRemoteInstance<WorldInstance, void*> {
public:
	friend class EditorSceneSerializer;

public:
	RemoteWorldInstance();
	~RemoteWorldInstance();

	__CLASS_DEFAULT_ALL(RemoteWorldInstance)

public:
	void draw_inspector() override;

	nlohmann::json serialize() const override;
};

#endif // DEBUG_FEATURES_ENABLE
