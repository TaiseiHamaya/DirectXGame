#ifdef DEBUG_FEATURES_ENABLE

#include "EditorCreateObjectCommand.h"

#include "../RemoteObject/EditorDeletedObjectPool.h"
#include "../Window/EditorSceneView.h"

EditorCreateObjectCommand::EditorCreateObjectCommand(Reference<IRemoteObject> parent_, std::unique_ptr<IRemoteObject> generatedObjectTemp_) {
	parent = parent_;
	generatedObjectTemp = std::move(generatedObjectTemp_);
	object = generatedObjectTemp;
}

void EditorCreateObjectCommand::execute() {
	generatedObjectTemp->on_spawn(parent);
	if (generatedObjectTemp) { // 初回
		// SceneViewに登録
		generatedObjectTemp->setup();
		parent->add_child(std::move(generatedObjectTemp));
	}
	else { // undo
		parent->add_child(deletedPool->get_deleted_force(object));
	}
}

void EditorCreateObjectCommand::undo() {
	object->on_destroy();
	deletedPool->delete_sequence(parent, object);
}

void EditorCreateObjectCommand::Setup(Reference<EditorDeletedObjectPool> deletedPool_) {
	deletedPool = deletedPool_;
}

#endif // DEBUG_FEATURES_ENABLE
