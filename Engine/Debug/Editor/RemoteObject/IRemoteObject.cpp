#include "IRemoteObject.h"

void IRemoteObject::reparent(Reference<IRemoteObject> remoteObject) {
	parent = remoteObject;
}

Reference<IRemoteObject> IRemoteObject::get_parent() const {
	return parent;
}
