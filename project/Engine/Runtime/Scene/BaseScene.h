#pragma once

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

#ifdef _DEBUG
	virtual void debug_update();
#endif // _DEBUG

private:
};