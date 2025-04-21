#pragma once

#include "Engine/Runtime/Scene/BaseSceneFactory.h"

enum SceneListDemo {
	Main,
};

class FactoryDemo final : public BaseSceneFactory {
public:
	std::unique_ptr<BaseScene> initialize_scene();
	std::unique_ptr<BaseScene> create_scene(int32_t next);
};
