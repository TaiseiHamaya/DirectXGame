#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "IRemoteObject.h"

#include <vector>

class FolderObject final : public IRemoteObject {
public:
	void initialize();

	void draw_inspector() override;

	void draw_hierarchy(Reference<IRemoteObject>& select) override;

private:
	std::vector<Reference<FolderObject>> childrenFolders;
	std::vector<Reference<class RemoteWorldInstance>> childrenInstances;
};

#endif // DEBUG_FEATURES_ENABLE
