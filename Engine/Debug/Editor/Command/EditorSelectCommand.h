#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "IEditorCommand.h"

#include "../Core/EditorSelectObject.h"

class EditorSelectCommand final : public IEditorCommand {
public:
	EditorSelectCommand(Reference<IRemoteObject> object, EditorSelectObjectBody::TransformData transform_ = {});
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

#endif // DEBUG_FEATURES_ENABLE
