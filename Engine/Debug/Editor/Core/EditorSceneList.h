#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include <string>
#include <unordered_set>

class EditorSceneList {
public:
	void initialize();
	void finalize();

public:
	bool scene_list_gui(std::string& current);

	void add_scene(const std::string& sceneName);

private:
	void export_scene_list();

private:
	std::unordered_set<std::string> sceneList;
};

#endif // DEBUG_FEATURES_ENABLE
