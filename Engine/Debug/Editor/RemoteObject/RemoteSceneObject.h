#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "IRemoteObject.h"

#include <vector>

class BaseScene;
class RemoteWorldObject;
class EditorSceneView;

class RemoteSceneObject final : public IRemoteObject {
public:
	friend class EditorSceneSerializer;

public:
	RemoteSceneObject();
	~RemoteSceneObject();

	__CLASS_DEFAULT_ALL(RemoteSceneObject)

public:
	void setup() override;

	void update_preview(Reference<RemoteWorldObject> world, Reference<Affine> parentAffine) override;

	void draw_inspector() override;

	void draw_hierarchy(Reference<const EditorSelectObject> select) override;

	std::unique_ptr<IRemoteObject> move_force(Reference<const IRemoteObject> child) override;

	void reparent(Reference<IRemoteObject> remoteObject) override;

	void add_child(std::unique_ptr<IRemoteObject> child) override;

	nlohmann::json serialize() const override;

	Reference<const RemoteWorldObject> query_world() const override;

public:
	size_t world_size() const;

	std::string name() const;

	const std::vector<std::unique_ptr<RemoteWorldObject>>& get_remote_worlds() const;

private:
	Reference<BaseScene> self;

	std::vector<std::unique_ptr<RemoteWorldObject>> remoteWorlds;

	EditorValueField<u32> numLayer{ "NumLayer", 1 };
};

#endif // DEBUG_FEATURES_ENABLE
