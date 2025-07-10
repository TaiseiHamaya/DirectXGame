#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "IEditorCommand.h"

#include <Library/Utility/Template/Reference.h>

template<typename Container>
	requires requires(Container c) { c.resize(size_t()); }
class EditorCommandResizeContainer final : public IEditorCommand {
public:
	EditorCommandResizeContainer(Reference<Container> container_, size_t next);
	~EditorCommandResizeContainer() = default;

	__CLASS_NON_COPYABLE(EditorCommandResizeContainer)

public:
	void execute() override;
	void undo() override;

private:
	Reference<Container> container;
	size_t prev;
	size_t next;
};

template<typename Container>
	requires requires(Container c) { c.resize(size_t()); }
inline EditorCommandResizeContainer<Container>::EditorCommandResizeContainer(Reference<Container> container_, size_t next) {
	prev = container_->size();
	container = container_;
	this->next = next;
}

template<typename Container>
	requires requires(Container c) { c.resize(size_t()); }
inline void EditorCommandResizeContainer<Container>::execute() {
	container->resize(next);
}

template<typename Container>
	requires requires(Container c) { c.resize(size_t()); }
inline void EditorCommandResizeContainer<Container>::undo() {
	container->resize(prev);
}

#endif // DEBUG_FEATURES_ENABLE
