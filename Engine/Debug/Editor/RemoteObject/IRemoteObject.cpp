#ifdef DEBUG_FEATURES_ENABLE

#include "IRemoteObject.h"

void IRemoteObject::reparent(Reference<IRemoteObject> remoteObject) {
	parent = remoteObject;
}

Reference<IRemoteObject> IRemoteObject::get_parent() const {
	return parent;
}

#endif // DEBUG_FEATURES_ENABLE
