#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "IEditorCommand.h"

class EditorCommandScopeBegin final : public IEditorCommand {
public:
	EditorCommandScopeBegin();
	~EditorCommandScopeBegin() override = default;

public:
	void execute() override;
	void undo() override;

private:
	bool isFirst{ true };
	const i32 level{ 0 };
};

class EditorCommandScopeEnd final : public IEditorCommand {
public:
	EditorCommandScopeEnd();
	~EditorCommandScopeEnd() override = default;

public:
	void execute() override;
	void undo() override;

private:
	const i32 level{ 0 };
};

#endif // DEBUG_FEATURES_ENABLE
