#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include <Library/Utility/Template/SingletonInterface.h>

#include <deque>
#include <optional>
#include <memory>

class IEditorCommand;

class EditorCommandInvoker final : public SingletonInterface<EditorCommandInvoker> {
	__CLASS_SINGLETON_INTERFACE(EditorCommandInvoker)

public:
	static void Execute(std::unique_ptr<IEditorCommand> command);
	static void Redo();
	static void Undo();
	static void ResetHistoryForce();

private:
	std::optional<u64> recent;
	std::deque<std::unique_ptr<IEditorCommand>> history;
};

#endif // DEBUG_FEATURES_ENABLE
