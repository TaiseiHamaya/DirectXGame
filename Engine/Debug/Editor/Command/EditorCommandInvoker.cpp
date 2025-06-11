#include "EditorCommandInvoker.h"

#include "IEditorCommand.h"

void EditorCommandInvoker::execute(std::unique_ptr<IEditorCommand> command) {
	command->execute(); // 実行
	if (recent.has_value()) {
		// undoした部分の履歴を削除
		while (recent.value() + 1 < history.size()) {
			history.pop_back();
		}
	}
	// 追加
	history.emplace_back(std::move(command));
	// Index調整
	if (recent.has_value()) {
		++recent.value();
	}
	else {
		recent = 0;
	}
}

void EditorCommandInvoker::redo() {
	if (!recent.has_value() || recent.value() + 1 == history.size()) {
		return; // 履歴がない or 先頭
	}
	++recent.value();
	history[recent.value()]->execute();
}

void EditorCommandInvoker::undo() {
	if (!recent.has_value()) {
		return;
	}
	history[recent.value()]->undo();
	if (recent.value() == 0) {
		recent = std::nullopt;
	}
	else {
		--recent.value();
	}
}
