#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "./IRemoteInstance.h"

#include "Engine/Module/World/WorldInstance/WorldInstance.h"

class RemoteWorldInstance : public IRemoteInstance<WorldInstance> {
public:
	friend class EditorSceneSerializer;

public:
	RemoteWorldInstance();
	~RemoteWorldInstance();

	__CLASS_DEFAULT_ALL(RemoteWorldInstance)

public:
	void draw_inspector() override;

	void draw_hierarchy(Reference<const EditorSelectObject> select) override;

private:
};

#endif // DEBUG_FEATURES_ENABLE
