#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "IRemoteObject.h"

#include <memory>
#include <string>
#include <vector>

class WorldManager;
class FolderObject;
class RemoteWorldInstance;

class RemoteWorldObject final : public IRemoteObject {
public:
	friend class EditorSceneSerializer;

public:
	RemoteWorldObject();
	~RemoteWorldObject();

	__CLASS_NON_COPYABLE(RemoteWorldObject)

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

	void on_spawn() override;

	void on_destroy() override;

	const std::string& world_name() const;

private:
	Reference<WorldManager> self;

	std::vector<std::unique_ptr<IRemoteObject>> children;
};

#endif // DEBUG_FEATURES_ENABLE
