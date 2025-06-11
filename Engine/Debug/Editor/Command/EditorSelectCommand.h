#pragma once

#include "IEditorCommand.h"

#include "../RemoteObject/IRemoteObject.h"

class EditorSelectCommand final : public IEditorCommand {
public:
	EditorSelectCommand() = default;
	~EditorSelectCommand() = default;

	__CLASS_NON_COPYABLE(EditorSelectCommand)

public:
	void execute() override;
	void undo() override;

private:
	Reference<IRemoteObject> next;
	Reference<IRemoteObject> prev;
};
