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

	void draw_hierarchy(Reference<const EditorSelectObject> select) override;

	std::unique_ptr<IRemoteObject> move_force(Reference<const IRemoteObject> child) override;

	void reparent(Reference<IRemoteObject> remoteObject) override;

	void add_child(std::unique_ptr<IRemoteObject> child) override;

	nlohmann::json serialize() const override;

public:
	std::string name() const;

private:
	Reference<BaseScene> self;

	std::vector<std::unique_ptr<RemoteWorldObject>> remoteWorlds;
};

#endif // DEBUG_FEATURES_ENABLE
