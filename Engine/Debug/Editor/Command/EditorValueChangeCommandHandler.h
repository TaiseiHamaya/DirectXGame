#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include <Library/Utility/Template/Reference.h>
#include <Library/Utility/Template/SingletonInterface.h>

#include <concepts>
#include <functional>

#include "EditorCommandInvoker.h"
#include "EditorValueChangeCommand.h"

class EditorValueChangeCommandHandler final : public SingletonInterface<EditorValueChangeCommandHandler> {
	__CLASS_SINGLETON_INTERFACE(EditorValueChangeCommandHandler)

private:
	static void Start(std::function<void(void)> endCallFunc);

public:
	static void End();

	static bool IsActive();

private:
	std::function<void(void)> endCallFunc{};

public:
	template<typename T>
		requires std::copyable<T>
	static void GenCommand(Reference<T> target);

	template<typename T>
		requires std::copyable<T>
	static void GenCommand(std::function<T&()> function);
};

template<typename T>
	requires std::copyable<T>
void EditorValueChangeCommandHandler::GenCommand(Reference<T> target) {
	EditorValueChangeCommand<T>* command
		= new EditorValueChangeCommand<T>(target);

	Start([command]() {
		command->prepare();
		EditorCommandInvoker::Execute(
			std::unique_ptr<EditorValueChangeCommand<T>>(command)
		);
	});
};

template<typename T>
	requires std::copyable<T>
void EditorValueChangeCommandHandler::GenCommand(std::function<T& ()> function) {
	EditorValueChangeCommandLambda<T>* command
		= new EditorValueChangeCommandLambda<T>(function);

	Start([command]() {
		command->prepare();
		EditorCommandInvoker::Execute(
			std::unique_ptr<EditorValueChangeCommandLambda<T>>(command)
		);
	});
};

#endif // DEBUG_FEATURES_ENABLE
