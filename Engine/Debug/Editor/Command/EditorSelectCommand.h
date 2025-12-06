#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "IEditorCommand.h"

#include "../Core/EditorSelectObject.h"

namespace szg {

class EditorSelectCommand final : public IEditorCommand {
public:
	EditorSelectCommand(Reference<IRemoteObject> object, EditorSelectObjectBody::TransformData transform_ = {});
	~EditorSelectCommand() = default;

	SZG_CLASS_MOVE_ONLY(EditorSelectCommand)

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

}; // szg

#endif // DEBUG_FEATURES_ENABLE
