#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "IRemoteObject.h"

#include <vector>

class WorldManager;
class FolderObject;
class RemoteWorldInstance;

class RemoteWorldRootObject final : public IRemoteObject {
public:
	RemoteWorldRootObject();
	~RemoteWorldRootObject();

public:
	RemoteWorldRootObject(const RemoteWorldRootObject&);
	RemoteWorldRootObject& operator=(const RemoteWorldRootObject&);
	RemoteWorldRootObject(RemoteWorldRootObject&&) noexcept;
	RemoteWorldRootObject& operator=(RemoteWorldRootObject&&) noexcept;

public:
	void initialize(Reference<WorldManager> self_);

	void draw_inspector() override;

	void draw_hierarchy(Reference<IRemoteObject>& select) override;

private:
	Reference<WorldManager> self;

	std::vector<FolderObject> childrenFolders;
	std::vector<RemoteWorldInstance> childrenInstances;
};

#endif // DEBUG_FEATURES_ENABLE
