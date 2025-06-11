#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "IEditorWindow.h"

#include <filesystem>
#include <memory>

#include <Library/Utility/Template/Reference.h>

#include "RemoteObject/RemoteSceneObject.h"

class IRemoteObject;
class EditorSelectObject;

class EditorHierarchy : public IEditorWindow {
public:
	void setup(Reference<EditorSelectObject> select_);
	void load(std::filesystem::path file);

	void draw() override;

private:
	std::unique_ptr<RemoteSceneObject> scene;

	std::string searchString;
	std::string menuString;

	Reference<EditorSelectObject> select{ nullptr };

	bool savedTrigger;
};

#endif // DEBUG_FEATURES_ENABLE
