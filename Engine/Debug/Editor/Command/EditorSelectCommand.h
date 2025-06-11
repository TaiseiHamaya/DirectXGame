#pragma once

#include "IEditorCommand.h"

#include "../EditorSelectObject.h"

class EditorSelectCommand final : public IEditorCommand {
public:
	EditorSelectCommand(Reference<IRemoteObject> object, Reference<Transform3D> transform = nullptr);
	~EditorSelectCommand() = default;

	__CLASS_NON_COPYABLE(EditorSelectCommand)

public:
	void execute() override;
	void undo() override;

private:
	EditorSelectObjectBody next;
	EditorSelectObjectBody prev;

public:
	static void Setup(Reference<EditorSelectObject> selectObject_);

private:
	static inline Reference<EditorSelectObject> selectObject{};
};
