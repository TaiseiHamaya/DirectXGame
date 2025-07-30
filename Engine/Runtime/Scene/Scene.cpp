#include "Scene.h"

void Scene::initialize() {
}

void Scene::update() {
	for (WorldCluster& world : worlds) {
		world.update();
	}
}

void Scene::draw() const {
}

void Scene::end_frame() {
	for (WorldCluster& world : worlds) {
		world.end_frame();
	}
}
