#include "Scene.h"

#include <filesystem>

void Scene::initialize(const std::string& sceneName) {
	for (const std::filesystem::directory_entry& entry : 
		std::filesystem::directory_iterator(std::format("./Game/Core/Scene/{}/Worlds", sceneName))) {
		WorldCluster world;

		world.initialize();
		world.setup(entry.path());

		worlds.emplace_back(std::move(world));
	}

	renderDAG.setup(sceneName, this);
}

void Scene::update() {
	for (WorldCluster& world : worlds) {
		world.update();
	}
}

void Scene::draw() const {
	renderDAG.render_entry_point();
}

void Scene::end_frame() {
	for (WorldCluster& world : worlds) {
		world.end_frame();
	}
}

Reference<WorldCluster> Scene::get_world(u32 index) {
	if( index >= worlds.size() ) {
		Warning("Try to reference world out of range index-\'{}\'.", index);
		return nullptr;
	}
	return worlds[index];
}
