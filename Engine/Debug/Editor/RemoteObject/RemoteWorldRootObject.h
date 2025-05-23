#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "IRemoteObject.h"

#include <memory>
#include <vector>

class WorldManager;
class FolderObject;
class RemoteWorldInstance;

class RemoteWorldRootObject final : public IRemoteObject {
public:
	RemoteWorldRootObject();
	~RemoteWorldRootObject();

	__CLASS_NON_COPYABLE(RemoteWorldRootObject)

public:
	void initialize(Reference<WorldManager> self_);

	void draw_inspector() override;

	void draw_hierarchy(Reference<IRemoteObject>& select) override;

private:
	Reference<WorldManager> self;

	std::vector<std::unique_ptr<IRemoteObject>> children;
};

#endif // DEBUG_FEATURES_ENABLE
