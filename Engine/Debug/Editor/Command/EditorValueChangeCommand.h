#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "IEditorCommand.h"

#include <Library/Utility/Template/Reference.h>

template<typename T>
class EditorValueChangeCommand final : public IEditorCommand {
public:
	EditorValueChangeCommand(Reference<T> target_);
	~EditorValueChangeCommand() = default;

public:
	void prepare();
	void execute() override;
	void undo() override;

private:
	Reference<T> target;
	T recent;
	T changed;
};

template<typename T>
EditorValueChangeCommand<T>::EditorValueChangeCommand(Reference<T> target_) {
	target = target_;
	recent = *target;
}

template<typename T>
void EditorValueChangeCommand<T>::prepare() {
	changed = *target;
}

template<typename T>
void EditorValueChangeCommand<T>::execute() {
	*target = changed;
}

template<typename T>
void EditorValueChangeCommand<T>::undo() {
	*target = recent;
}

#endif // DEBUG_FEATURES_ENABLE
