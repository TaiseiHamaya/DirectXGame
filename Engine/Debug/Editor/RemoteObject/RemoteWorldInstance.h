#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "IRemoteObject.h"

#include <vector>

#include "Engine/Module/World/WorldInstance/WorldInstance.h"

class RemoteWorldInstance;

class RemoteWorldInstance : public IRemoteObject {
public:
	RemoteWorldInstance();
	~RemoteWorldInstance();

	__CLASS_DEFAULT_ALL(RemoteWorldInstance)

public:
	void initialize(Reference<WorldInstance> self_);

	void draw_inspector() override;

	void draw_hierarchy(Reference<IRemoteObject>& select) override;

private:
	Reference<WorldInstance> self;

	Reference<class FolderObject> parentFolder;

	std::vector<Reference<FolderObject>> childrenFolders;
	std::vector<Reference<RemoteWorldInstance>> childrenInstances;
};

#endif // DEBUG_FEATURES_ENABLE
