#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "IEditorCommand.h"

#include <Library/Utility/Template/Reference.h>

#include "../RemoteObject/IRemoteObject.h"

class EditorDeletedObjectPool;

class EditorDeleteObjectCommand final : public IEditorCommand {
public:
	EditorDeleteObjectCommand(Reference<IRemoteObject> object);
	~EditorDeleteObjectCommand() = default;

public:
	void execute() override;
	void undo() override;

private:
	Reference<IRemoteObject> parent;
	Reference<IRemoteObject> object;

public:
	static void Setup(Reference<EditorDeletedObjectPool> deletedPool_);

private:
	static inline Reference<EditorDeletedObjectPool> deletedPool{};
};

#endif // DEBUG_FEATURES_ENABLE
