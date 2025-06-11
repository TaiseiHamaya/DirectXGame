#pragma once

#include <Library/Utility/Tools/ConstructorMacro.h>

#include <deque>
#include <optional>
#include <memory>

class IEditorCommand;

class EditorCommandInvoker {
public:
	EditorCommandInvoker() = default;
	~EditorCommandInvoker() = default;

	__CLASS_NON_COPYABLE(EditorCommandInvoker)

public:
	void execute(std::unique_ptr<IEditorCommand> command);
	void redo();
	void undo();

private:
	std::optional<u64> recent;
	std::deque<std::unique_ptr<IEditorCommand>> history;
};
