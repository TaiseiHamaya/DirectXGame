#ifdef DEBUG_FEATURES_ENABLE

#include "EditorDeleteObjectCommand.h"

#include "../RemoteObject/EditorDeletedObjectPool.h"

EditorDeleteObjectCommand::EditorDeleteObjectCommand(Reference<IRemoteObject> object_) {
	object = object_;
	parent = object->get_parent();
}

void EditorDeleteObjectCommand::execute() {
	object->on_destroy();
	deletedPool->delete_sequence(parent, object);
}

void EditorDeleteObjectCommand::undo() {
	auto instance = deletedPool->get_deleted_force(object);
	instance->on_spawn(parent);
	parent->add_child(std::move(instance));
}

void EditorDeleteObjectCommand::Setup(Reference<EditorDeletedObjectPool> deletedPool_) {
	deletedPool = deletedPool_;
}

#endif // DEBUG_FEATURES_ENABLE
