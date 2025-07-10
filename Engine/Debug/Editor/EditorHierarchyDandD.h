#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include <memory>

#include <Library/Utility/Template/Reference.h>
#include <Library/Utility/Template/SingletonInterface.h>

#include "RemoteObject/IRemoteObject.h"

class EditorCommandReparent;

class EditorHierarchyDandD : public SingletonInterface<EditorHierarchyDandD> {
	__CLASS_SINGLETON_INTERFACE(EditorHierarchyDandD)

public:
	struct DragData {
		Reference<IRemoteObject> parent;
		Reference<IRemoteObject> dragging;
	};

public:
	static void CheckDandD(Reference<IRemoteObject> self, Reference<IRemoteObject> parent);
	static void BeginDrag(Reference<IRemoteObject> self, Reference<IRemoteObject> parent);
	static void EndDrag(Reference<IRemoteObject> target);

	static void ExecuteReparent();

private:
	DragData dragData{};
	std::unique_ptr<EditorCommandReparent> command{ nullptr };
};

#endif // DEBUG_FEATURES_ENABLE
