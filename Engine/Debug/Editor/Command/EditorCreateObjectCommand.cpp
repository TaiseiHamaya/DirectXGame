#include "EditorCreateObjectCommand.h"

#include "../RemoteObject/EditorDeletedObjectPool.h"

EditorCreateObjectCommand::EditorCreateObjectCommand(Reference<IRemoteObject> parent_, std::unique_ptr<IRemoteObject> generatedObjectTemp_) {
	parent = parent_;
	generatedObjectTemp = std::move(generatedObjectTemp_);
	object = generatedObjectTemp;
}

void EditorCreateObjectCommand::execute() {
	if (generatedObjectTemp) { // 初回
		parent->add_child(std::move(generatedObjectTemp));
	}
	else { // undo
		parent->add_child(deletedPool->get_deleted_force(object));
	}
}

void EditorCreateObjectCommand::undo() {
	deletedPool->delete_sequence(parent, object);
}

void EditorCreateObjectCommand::Setup(Reference<EditorDeletedObjectPool> deletedPool_) {
	deletedPool = deletedPool_;
}
