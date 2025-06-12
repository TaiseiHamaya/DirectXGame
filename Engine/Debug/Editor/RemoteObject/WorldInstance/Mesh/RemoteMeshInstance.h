#pragma once

#include "../IRemoteInstance.h"

class StaticMeshInstance;

class RemoteMeshInstance final : public IRemoteInstance<StaticMeshInstance> {
public:
	friend class EditorSceneSerializer;

public:
	RemoteMeshInstance() = default;
	~RemoteMeshInstance() = default;

public:

public:
	void draw_inspector() override;

	void draw_hierarchy(Reference<const EditorSelectObject> select) override;

	nlohmann::json serialize() const override;
};
