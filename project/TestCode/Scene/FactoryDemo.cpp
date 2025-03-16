#include "FactoryDemo.h"

#include "SceneDemo.h"

std::unique_ptr<BaseScene> FactoryDemo::initialize_scene() {
	return std::make_unique<SceneDemo>();
}

std::unique_ptr<BaseScene> FactoryDemo::create_scene(int32_t next) {
	switch (next) {
	case SceneListDemo::Main:
		return std::make_unique<SceneDemo>();
		break;

	default:
		return std::make_unique<SceneDemo>();
		break;
	}
}
