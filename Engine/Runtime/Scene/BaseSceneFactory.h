#pragma once

#include <memory>

namespace szg {

class Scene;
class BaseScene;

class BaseSceneFactory {
public:
	BaseSceneFactory() = default;
	virtual ~BaseSceneFactory() = default;

public:
	[[deprecated]] virtual std::unique_ptr<BaseScene> initialize_scene() = 0;
	[[deprecated]] virtual std::unique_ptr<BaseScene> create_scene(i32 next) = 0;

public:
	virtual std::unique_ptr<Scene> initialize_scene2() = 0;
	virtual std::unique_ptr<Scene> create_scene2(i32 next) = 0;
};


}; // szg
