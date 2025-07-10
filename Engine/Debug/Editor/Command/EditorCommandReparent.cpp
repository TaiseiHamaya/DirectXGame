#ifdef DEBUG_FEATURES_ENABLE

#include "EditorCommandReparent.h"

#include "Engine/Application/Output.h"

EditorCommandReparent::EditorCommandReparent(Reference<IRemoteObject> target_, Reference<IRemoteObject> recent, Reference<IRemoteObject> next) {
	if(!target_) {
		Warning("Target is nullptr.");
		return;
	}
	if(!recent) {
		Warning("Recent parent is nullptr.");
		return;
	}
	if(!next) {
		Warning("Next parent is nullptr.");
		return;
	}
	target = target_;
	recentParent = recent;
	nextParent = next;
}

void EditorCommandReparent::execute() {
	auto instance = recentParent->move_force(target);
	if (!instance) {
		Warning("Failed to move target to next parent.");
		return;
	}

	nextParent->add_child(std::move(instance));
}

void EditorCommandReparent::undo() {
	auto instance = nextParent->move_force(target);
	if (!instance) {
		Warning("Failed to move target back to recent parent.");
		return;
	}

	recentParent->add_child(std::move(instance));
}

#endif // DEBUG_FEATURES_ENABLE
