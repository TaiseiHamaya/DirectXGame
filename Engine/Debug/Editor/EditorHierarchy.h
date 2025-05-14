#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "IEditorWindow.h"

#include <filesystem>

#include <Library/Utility/Template/Reference.h>

#include "RemoteObject/RemoteSceneObject.h"

class IRemoteObject;

class EditorHierarchy : public IEditorWindow {
public:
	void load(std::filesystem::path file);

	void draw() override;

	Reference<IRemoteObject> get_select() const;

private:
	RemoteSceneObject scene;

	std::string searchString;
	std::string menuString;

	Reference<IRemoteObject> select{ nullptr };

	bool savedTrigger;
};

#endif // DEBUG_FEATURES_ENABLE
