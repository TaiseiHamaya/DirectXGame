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
	/// <summary>
	/// ロード処理
	/// </summary>
	virtual void load();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void initialize();

	/// <summary>
	/// 終了処理
	/// </summary>
	virtual void finalize();

	/// <summary>
	/// SceneStackからPopされた際に実行
	/// </summary>
	virtual void popped();

	/// <summary>
	/// フレーム開始時の処理
	/// </summary>
	virtual void begin();

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void update() = 0;

	/// <summary>
	/// 描画準備
	/// </summary>
	virtual void begin_rendering();

	/// <summary>
	/// 描画前処理(描画に反映されない)
	/// </summary>
	virtual void late_update();

	/// <summary>
	/// 描画実行
	/// </summary>
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