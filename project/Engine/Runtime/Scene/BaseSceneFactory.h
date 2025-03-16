#pragma once

#include <memory>

class BaseScene;

class BaseSceneFactory {
public:
	BaseSceneFactory() = default;
	virtual ~BaseSceneFactory() = default;

public:
	virtual std::unique_ptr<BaseScene> initialize_scene() = 0;
	virtual std::unique_ptr<BaseScene> create_scene(int32_t next) = 0;
};

