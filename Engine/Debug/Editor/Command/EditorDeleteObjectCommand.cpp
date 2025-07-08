#ifdef DEBUG_FEATURES_ENABLE

#include "EditorDeleteObjectCommand.h"

#include "../RemoteObject/EditorDeletedObjectPool.h"

EditorDeleteObjectCommand::EditorDeleteObjectCommand(Reference<IRemoteObject> object_) {
	object = object_;
	parent = object->get_parent();
}

void EditorDeleteObjectCommand::execute() {
	deletedPool->delete_sequence(parent, object);
}

void EditorDeleteObjectCommand::undo() {
	parent->add_child(std::move(deletedPool->get_deleted_force(object)));
}

void EditorDeleteObjectCommand::Setup(Reference<EditorDeletedObjectPool> deletedPool_) {
	deletedPool = deletedPool_;
}

#endif // DEBUG_FEATURES_ENABLE
