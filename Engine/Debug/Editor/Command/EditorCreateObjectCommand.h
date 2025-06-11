#pragma once

#include "IEditorCommand.h"

#include <Library/Utility/Template/Reference.h>

#include "../RemoteObject/IRemoteObject.h"

class EditorDeletedObjectPool;

class EditorCreateObjectCommand final : public IEditorCommand {
public:
	EditorCreateObjectCommand(Reference<IRemoteObject> parent_, std::unique_ptr<IRemoteObject> generatedObjectTemp);
	~EditorCreateObjectCommand() = default;

public:
	void execute() override;
	void undo() override;

private:
	Reference<IRemoteObject> parent;
	Reference<IRemoteObject> object;

	std::unique_ptr<IRemoteObject> generatedObjectTemp;

public:
	static void Setup(Reference<EditorDeletedObjectPool> deletedPool_);

private:
	static inline Reference<EditorDeletedObjectPool> deletedPool{};
};

