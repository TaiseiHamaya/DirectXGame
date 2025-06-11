#include "EditorObjectMoveCommand.h"

EditorObjectMoveCommand::EditorObjectMoveCommand(Reference<Transform3D> transform) {
	target = transform;
	prev.copy(*target);
}

void EditorObjectMoveCommand::preprocess() {
	next.copy(*target);
}

void EditorObjectMoveCommand::execute() {
	target->copy(next);
}

void EditorObjectMoveCommand::undo() {
	target->copy(prev);
}
