#pragma once

#include <vector>

#include "Engine/Module/World/WorldManager.h"

/// <summary>
/// シーンの基底クラス
/// </summary>
class BaseScene {
public:
	BaseScene() = default;
	virtual ~BaseScene() = default;

private: /// --------------コピー禁止--------------
	BaseScene(const BaseScene&) = delete;
	BaseScene& operator=(const BaseScene&) = delete;

public:
	virtual void load();
	virtual void initialize();
	virtual void finalize();
	virtual void popped();

	virtual void begin();
	virtual void update() = 0;
	virtual void begin_rendering();
	virtual void late_update();

	virtual void draw() const = 0;

#ifdef DEBUG_FEATURES_ENABLE
	virtual void debug_update();
#endif // _DEBUG

public:
	u64 world_size() const;
	Reference<WorldManager> get_world(u64 index);

protected:
	std::vector<WorldManager> worlds;
};