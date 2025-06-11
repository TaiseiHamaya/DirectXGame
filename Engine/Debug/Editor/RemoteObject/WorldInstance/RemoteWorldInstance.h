#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include <memory>

#include "./IRemoteInstance.h"

#include "Engine/Module/World/WorldInstance/WorldInstance.h"

class EditorObjectMoveCommand;

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
	std::unique_ptr<EditorObjectMoveCommand> moveCommand;
};

#endif // DEBUG_FEATURES_ENABLE
