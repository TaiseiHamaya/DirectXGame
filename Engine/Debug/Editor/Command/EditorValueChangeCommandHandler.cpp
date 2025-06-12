#include "EditorValueChangeCommandHandler.h"

#include <Engine/Application/Output.h>

void EditorValueChangeCommandHandler::Start(std::function<void(void)> endCallFunc) {
	auto& instance = GetInstance();
	if (instance.endCallFunc) {
		Error("Change command was called but the previous command has not yet finished.");
		return;
	}

	GetInstance().endCallFunc = endCallFunc;
}

void EditorValueChangeCommandHandler::End() {
	auto& instance = GetInstance();
	instance.endCallFunc();
	instance.endCallFunc = nullptr;
}

bool EditorValueChangeCommandHandler::IsActive() {
	return bool(GetInstance().endCallFunc);
}
