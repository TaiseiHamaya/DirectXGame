#pragma once

#include "IEditorCommand.h"

#include <Library/Math/Transform3D.h>
#include <Library/Utility/Template/Reference.h>

class EditorObjectMoveCommand final : public IEditorCommand {
public:
	EditorObjectMoveCommand(Reference<Transform3D> transform);
	~EditorObjectMoveCommand() = default;

public:
	void preprocess();

	void execute() override;
	void undo() override;

private:
	Reference<Transform3D> target;
	Transform3D prev;
	Transform3D next;
};

