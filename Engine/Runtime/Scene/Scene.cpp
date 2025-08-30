#include "Scene.h"

#include <filesystem>

void Scene::load_asset() {
}

void Scene::initialize() {
}

void Scene::setup() {
	// フォルダ内のワールドを全て読み込む
	for (const std::filesystem::directory_entry& entry :
		std::filesystem::directory_iterator(std::format("./Game/Core/Scene/{}/Worlds", sceneName))) {
		WorldCluster world;

		world.initialize();
		world.setup(entry.path());

		worlds.emplace_back(std::move(world));
	}
	WarningIf(worlds.empty(), "Scene-\'{}\' has no worlds.", sceneName);

	// 描画パスの初期化
	renderDAG.setup(sceneName, this);
}

void Scene::update() {
	for (WorldCluster& world : worlds) {
		world.update();
	}
}

void Scene::pre_draw() {
	for(WorldCluster& world : worlds ) {
		world.pre_draw();
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

void Scene::set_name(const std::string& name) {
	sceneName = name;
}

std::string_view Scene::name() const noexcept {
	return sceneName;
}
