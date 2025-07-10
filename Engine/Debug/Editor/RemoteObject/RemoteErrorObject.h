#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "IRemoteObject.h"

class RemoteErrorObject final : public IRemoteObject {
public:
	RemoteErrorObject(const std::string& msg);
	~RemoteErrorObject() = default;

	__CLASS_DEFAULT_ALL(RemoteErrorObject)

public:
	void draw_inspector() override;

	void draw_hierarchy(Reference<const EditorSelectObject> select) override;

	std::unique_ptr<IRemoteObject> move_force(Reference<const IRemoteObject> child) override;

	void reparent(Reference<IRemoteObject> remoteObject) override;

	void add_child(std::unique_ptr<IRemoteObject> child) override;

	nlohmann::json serialize() const override;

	void set_editor_world_view(Reference<EditorWorldView>, Reference<const Affine>) override {};

private:
	std::string errorMessage;
};

#endif // DEBUG_FEATURES_ENABLE
