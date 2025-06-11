#ifdef DEBUG_FEATURES_ENABLE

#include "EditorInspector.h"

#include <imgui.h>

#include "EditorSelectObject.h"
#include "RemoteObject/IRemoteObject.h"

void EditorInspector::initialize() {
	isActive = true;
}

void EditorInspector::setup(Reference<EditorSelectObject> selectObject_) {
	selectObject = selectObject_;
}

void EditorInspector::draw() {
	if (!isActive) {
		return;
	}

	int flags = 0;
	ImGui::Begin("Inspector", &isActive, flags);

	Reference<IRemoteObject> select = selectObject->get_item().object;

	if (select) {
		select->draw_inspector();
	}

	ImGui::End();
}

#endif // DEBUG_FEATURES_ENABLE
