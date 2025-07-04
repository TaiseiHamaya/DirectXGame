#ifdef DEBUG_FEATURES_ENABLE

#include "EditorSelectObject.h"

#include "RemoteObject/IRemoteObject.h"

void EditorSelectObject::set_item(Reference<IRemoteObject> object_, Reference<Transform3D> transform_) {
	select.object = object_;
	select.transform = transform_;
}

const EditorSelectObjectBody& EditorSelectObject::get_item() {
	return select;
}

bool EditorSelectObject::is_selected(Reference<const IRemoteObject> rhs) const {
	return select.object == rhs;
}

#endif // DEBUG_FEATURES_ENABLE
