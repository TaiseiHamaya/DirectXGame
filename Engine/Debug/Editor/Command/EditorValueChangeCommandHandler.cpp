#ifdef DEBUG_FEATURES_ENABLE

#include "EditorValueChangeCommandHandler.h"

#include <Engine/Application/Output.h>

void EditorValueChangeCommandHandler::Start(std::function<void(void)> endCallFunc) {
	auto& instance = GetInstance();
	if (instance.endCallFunc) {
		Warning("Change command was called but the previous command has not yet finished.");

		End();
	}

	GetInstance().endCallFunc = endCallFunc;
}

void EditorValueChangeCommandHandler::End() {
	auto& instance = GetInstance();
	if (!instance.endCallFunc) {
		Warning("EditorValueChangeCommandHandler::End() was called but ValueChangeCommand has not yet generated.");
		return;
	}
	instance.endCallFunc();
	instance.endCallFunc = nullptr;
}

bool EditorValueChangeCommandHandler::IsActive() {
	return bool(GetInstance().endCallFunc);
}

#endif // DEBUG_FEATURES_ENABLE
