#pragma once

#include "IRemoteObject.h"

class RemoteErrorObject final : public IRemoteObject {
public:
	RemoteErrorObject() = default;
	~RemoteErrorObject() = default;

	__CLASS_DEFAULT_ALL(RemoteErrorObject)

public:
	void draw_inspector() override;

	void draw_hierarchy(Reference<EditorSelectObject> select) override;
};
