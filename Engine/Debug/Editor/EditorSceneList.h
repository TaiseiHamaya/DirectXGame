#pragma once

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
	std::unordered_set<std::string> sceneList;
};
