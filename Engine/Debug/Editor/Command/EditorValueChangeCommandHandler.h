#pragma once

#include <Library/Utility/Template/SingletonInterface.h>
#include <Library/Utility/Template/Reference.h>

#include <functional>

#include "EditorValueChangeCommand.h"
#include "EditorCommandInvoker.h"

class EditorValueChangeCommandHandler final : public SingletonInterface<EditorValueChangeCommandHandler> {
	__CLASS_SINGLETON_INTERFACE(EditorValueChangeCommandHandler)

public:
	static void Start(std::function<void(void)> endCallFunc);
	static void End();

	static bool IsActive();

private:
	std::function<void(void)> endCallFunc{};

public:
	template<typename T, void (T::* CopyFunc)(const T&) = T::operator=()>
	static void GenCommand(Reference<T> target);
};

template<typename T, void (T::* CopyFunc)(const T&)>
void EditorValueChangeCommandHandler::GenCommand(Reference<T> target) {
	EditorValueChangeCommand<T, CopyFunc>* command
		= new EditorValueChangeCommand<T, CopyFunc>(target);

	Start([command]() {
		command->prepare();
		EditorCommandInvoker::Execute(
			std::unique_ptr<EditorValueChangeCommand<T, CopyFunc>>(command)
		);
	});
};
