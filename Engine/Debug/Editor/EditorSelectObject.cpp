#include "EditorSelectObject.h"

#include "RemoteObject/IRemoteObject.h"

void EditorSelectObject::set_item(Reference<IRemoteObject> object_, Reference<Transform3D> transform_) {
	select.object = object_;
	select.transform = transform_;
}

const EditorSelectObject::SelectObject& EditorSelectObject::get_item() {
	return select;
}

bool EditorSelectObject::is_selected(Reference<const IRemoteObject> rhs) const {
	return select.object == rhs;
}
