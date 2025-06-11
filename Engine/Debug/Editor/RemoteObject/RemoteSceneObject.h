#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "IRemoteObject.h"

#include <vector>

class BaseScene;
class RemoteWorldObject;

class RemoteSceneObject final : public IRemoteObject {
public:
	friend class EditorSceneSerializer;

public:
	RemoteSceneObject();
	~RemoteSceneObject();

	__CLASS_DEFAULT_ALL(RemoteSceneObject)

public:
	void draw_inspector() override;

	void draw_hierarchy(Reference<EditorSelectObject> select) override;

private:
	Reference<BaseScene> self;
	std::vector<std::unique_ptr<RemoteWorldObject>> remoteWorlds;
};

#endif // DEBUG_FEATURES_ENABLE
