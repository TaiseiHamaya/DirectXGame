#pragma once

#include "IEditorCommand.h"

#include <Library/Utility/Template/Reference.h>

template<typename T, void (T::* CopyFunc)(const T&) = T::operator=()>
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

template<typename T, void (T:: *CopyFunc)(const T&)>
EditorValueChangeCommand<T, CopyFunc>::EditorValueChangeCommand(Reference<T> target_) {
	target = target_;
	(recent.*CopyFunc)(*target);
}

template<typename T, void (T::* CopyFunc)(const T&)>
void EditorValueChangeCommand<T, CopyFunc>::prepare() {
	(changed.*CopyFunc)(*target);
}

template<typename T, void (T::* CopyFunc)(const T&)>
void EditorValueChangeCommand<T, CopyFunc>::execute() {
	((target.ptr())->*CopyFunc)(changed);
}

template<typename T, void (T::* CopyFunc)(const T&)>
void EditorValueChangeCommand<T, CopyFunc>::undo() {
	((target.ptr())->*CopyFunc)(recent);
}
