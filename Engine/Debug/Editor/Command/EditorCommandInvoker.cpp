#include "EditorCommandInvoker.h"

#include "IEditorCommand.h"

void EditorCommandInvoker::Execute(std::unique_ptr<IEditorCommand> command) {
	command->execute(); // 実行
	auto& instance = GetInstance();
	if (instance.recent.has_value()) {
		// undoした部分の履歴を削除
		while (instance.recent.value() + 1 < instance.history.size()) {
			instance.history.pop_back();
		}
	}
	else {
		instance.history.clear();
	}
	// 追加
	instance.history.emplace_back(std::move(command));
	// Index調整
	if (instance.recent.has_value()) {
		++instance.recent.value();
	}
	else {
		instance.recent = 0;
	}
}

void EditorCommandInvoker::Redo() {
	auto& instance = GetInstance();
	if (instance.history.empty() || (instance.recent.has_value() && instance.recent.value() + 1 == instance.history.size())) {
		return; // 履歴がない or 先頭
	}
	if (instance.recent.has_value()) {
		++instance.recent.value();
	}
	else {
		instance.recent = 0;
	}
	instance.history[instance.recent.value()]->execute();
}

void EditorCommandInvoker::Undo() {
	auto& instance = GetInstance();
	if (!instance.recent.has_value()) {
		return;
	}
	instance.history[instance.recent.value()]->undo();
	if (instance.recent.value() == 0) {
		instance.recent = std::nullopt;
	}
	else {
		--instance.recent.value();
	}
}
