#include "Scene.h"

void Scene::initialize() {
}

void Scene::begin() {
}

void Scene::update() {
	for (WorldManager& world : worlds) {
		//world.update_matrix();
	}
}

void Scene::transfer() {
	for (WorldManager& world : worlds) {
		world.update_matrix();
	}
}

void Scene::late_update() {
}

void Scene::draw() const {
}
