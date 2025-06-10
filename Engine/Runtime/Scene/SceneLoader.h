#pragma once

#include <filesystem>

class SceneLoader {

public:


public:
	void load(std::filesystem::path file);

private:
	void create_instance_recursion();
};
