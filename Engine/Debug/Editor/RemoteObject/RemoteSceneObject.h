#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "IRemoteObject.h"

#include <vector>

class BaseScene;
class RemoteWorldRootObject;

class RemoteSceneObject final : public IRemoteObject {
public:
	RemoteSceneObject();
	~RemoteSceneObject();

	__CLASS_DEFAULT_ALL(RemoteSceneObject)

public:
	void initialize(Reference<BaseScene> self_);

	void draw_inspector() override;

	void draw_hierarchy(Reference<IRemoteObject>& select) override;

private:
	Reference<BaseScene> self;
	std::vector<RemoteWorldRootObject> worldRoots;
};

#endif // DEBUG_FEATURES_ENABLE
