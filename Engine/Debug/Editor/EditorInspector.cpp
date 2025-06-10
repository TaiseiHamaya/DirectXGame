#ifdef DEBUG_FEATURES_ENABLE

#include "EditorInspector.h"

#include <imgui.h>

#include "EditorHierarchy.h"
#include "RemoteObject/IRemoteObject.h"

void EditorInspector::initialize() {
	isActive = true;
}

void EditorInspector::start(Reference<EditorHierarchy> hierarchyRoot_) {
	hierarchyRoot = hierarchyRoot_;
}

void EditorInspector::draw() {
	if (!isActive) {
		return;
	}

	int flags = 0;
	ImGui::Begin("Inspector", &isActive, flags);

	Reference<IRemoteObject> select = hierarchyRoot->get_select();

	if (select) {
		select->draw_inspector();
	}

	ImGui::End();
}

#endif // DEBUG_FEATURES_ENABLE
