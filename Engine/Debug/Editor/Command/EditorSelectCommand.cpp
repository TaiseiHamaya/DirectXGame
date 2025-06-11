#include "EditorSelectCommand.h"

EditorSelectCommand::EditorSelectCommand(Reference<IRemoteObject> object, Reference<Transform3D> transform) {
	next.object = object;
	next.transform = transform;
	prev = selectObject->get_item();
}

void EditorSelectCommand::execute() {
	selectObject->set_item(next.object, next.transform);
}

void EditorSelectCommand::undo() {
	selectObject->set_item(prev.object, prev.transform);
}

void EditorSelectCommand::Setup(Reference<EditorSelectObject> selectObject_) {
	selectObject = selectObject_;
}
