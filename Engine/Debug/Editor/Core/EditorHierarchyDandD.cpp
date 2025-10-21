#ifdef DEBUG_FEATURES_ENABLE

#include "EditorHierarchyDandD.h"

#include <imgui.h>

#include "Engine/Application/Logger.h"
#include "Engine/Debug/Editor/Command/EditorCommandInvoker.h"
#include "Engine/Debug/Editor/Command/EditorCommandReparent.h"

void EditorHierarchyDandD::CheckDandD(Reference<IRemoteObject> self, Reference<IRemoteObject> parent) {
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
		BeginDrag(self, parent);
		ImGui::EndDragDropSource();
	}
	if (ImGui::BeginDragDropTarget()) {
		EndDrag(self);
		ImGui::EndDragDropTarget();
	}
}

void EditorHierarchyDandD::BeginDrag(Reference<IRemoteObject> self, Reference<IRemoteObject> parent) {
	if (!parent || !self) {
		return;
	}
	auto& instance = GetInstance();
	instance.dragData.parent = parent;
	instance.dragData.dragging = self;
	if (ImGui::GetDragDropPayload() == nullptr) {
		ImGui::SetDragDropPayload("EditorHierarchyDandD", &instance.dragData, sizeof(DragData), ImGuiCond_Once);
		szgInformation("Begin drag&drop.");
	}
}

void EditorHierarchyDandD::EndDrag(Reference<IRemoteObject> target) {
	auto& instance = GetInstance();
	if (!target) {
		szgError("Get ImGui drag&drop payload but target is nullptr.");
		return;
	}
	const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("EditorHierarchyDandD");
	if (!payload) {
		return;
	}
	if (payload->DataSize != sizeof(DragData)) {
		szgError("Get ImGui drag&drop payload but DragData size is mismatch.");
		return;
	}

	if (instance.dragData.parent == target) {
		// 同じ位置にドロップした場合は何もしない
		szgInformation("Drop to same parent.");
		return;
	}

	Reference<IRemoteObject> targetParent = target->get_parent();
	while (targetParent) {
		if (targetParent == instance.dragData.dragging) {
			// ドロップ先がターゲットの子孫である場合は何もしない
			szgWarning("Can't drop to target parent.");
			return;
		}
		targetParent = targetParent->get_parent();
	}

	instance.command =
		std::make_unique<EditorCommandReparent>(instance.dragData.dragging, instance.dragData.parent, target);
	szgInformation("Successed drag&drop.");
}

void EditorHierarchyDandD::ExecuteReparent() {
	auto& instance = GetInstance();
	if (!instance.command) {
		return;
	}
	EditorCommandInvoker::Execute(
		std::move(instance.command)
	);
}

#endif // DEBUG_FEATURES_ENABLE
