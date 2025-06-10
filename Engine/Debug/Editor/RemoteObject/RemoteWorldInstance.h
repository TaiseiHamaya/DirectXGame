#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "IRemoteObject.h"

#include <vector>

#include "Engine/Module/World/WorldInstance/WorldInstance.h"

class RemoteWorldInstance;

class RemoteWorldInstance : public IRemoteObject {
public:
	friend class EditorSceneSerializer;

public:
	RemoteWorldInstance();
	~RemoteWorldInstance();

	__CLASS_DEFAULT_ALL(RemoteWorldInstance)

public:
	void draw_inspector() override;

	void draw_hierarchy(Reference<IRemoteObject>& select) override;

private:
	Reference<WorldInstance> self;

	std::vector<std::unique_ptr<IRemoteObject>> children;
};

#endif // DEBUG_FEATURES_ENABLE
