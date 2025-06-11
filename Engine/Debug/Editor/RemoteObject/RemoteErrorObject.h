#pragma once

#include "IRemoteObject.h"

class RemoteErrorObject final : public IRemoteObject {
public:
	RemoteErrorObject() = default;
	~RemoteErrorObject() = default;

	__CLASS_DEFAULT_ALL(RemoteErrorObject)

public:
	void draw_inspector() override;

	void draw_hierarchy(Reference<const EditorSelectObject> select) override;

	std::unique_ptr<IRemoteObject> move_force(Reference<const IRemoteObject> child) override;

	void reparent(Reference<IRemoteObject> remoteObject) override;

	void add_child(std::unique_ptr<IRemoteObject> child) override;

	nlohmann::json serialize() const override;
};
