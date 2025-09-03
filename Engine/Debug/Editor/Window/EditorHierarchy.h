#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "IEditorWindow.h"

#include <memory>
#include <string>

#include <Library/Utility/Template/Reference.h>

#include "../RemoteObject/RemoteSceneObject.h"

class IRemoteObject;
class EditorSelectObject;
class EditorSceneView;

class EditorHierarchy : public IEditorWindow {
public:
	void setup(Reference<EditorSelectObject> select_, Reference<EditorSceneView> sceneView);

	void finalize();

	void update_preview();

	void load(const std::string& sceneName);
	nlohmann::json save() const;

	void draw() override;

public:
	std::string current_scene_name();
	const std::vector<std::unique_ptr<RemoteWorldObject>>& world_list() const;

private:
	std::unique_ptr<RemoteSceneObject> scene;

	std::string searchString;
	std::string menuString;

	Reference<EditorSelectObject> select{ nullptr };
	Reference<EditorSceneView> sceneView{ nullptr };

	bool savedTrigger;
};

#endif // DEBUG_FEATURES_ENABLE
