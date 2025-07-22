#ifdef DEBUG_FEATURES_ENABLE

#include "IRemoteObject.h"

void IRemoteObject::reparent(Reference<IRemoteObject> remoteObject) {
	parent = remoteObject;
}

Reference<const RemoteWorldObject> IRemoteObject::query_world() const {
	if (!parent) {
		return nullptr;
	}
	return parent->query_world();
}

Reference<IRemoteObject> IRemoteObject::get_parent() const {
	return parent;
}

void IRemoteObject::Setup(Reference<EditorSceneView> sceneView) {
	IRemoteObject::sceneView = sceneView;
}

#endif // DEBUG_FEATURES_ENABLE
