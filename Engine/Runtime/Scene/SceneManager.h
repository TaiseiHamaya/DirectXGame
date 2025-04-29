#pragma once

class BaseScene;
class BaseSceneFactory;

#include <deque>
#include <memory>

#include <Library/Utility/Template/TimedCall.h>
#include <Library/Utility/Template/Reference.h>

class TimestampProfiler;

/// <summary>
/// シーン管理用クラス
/// </summary>
class SceneManager final {
private:
	SceneManager() = default;
	~SceneManager() = default;

public:
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;

public:
	static SceneManager& GetInstance() noexcept;

public:
	static void Initialize();
	static void Finalize() noexcept;

	static void Begin();
	static void Update();
	static void Draw();
	//static void Debug();
	static void SetSceneChange(
		i32 next,
		r32 interval,
		bool isStackInitialScene_ = false,
		bool isStopLoad = true
	);
	static void PopScene(r32 interval);

public:
	static bool IsEndProgram() noexcept;

	static const std::deque<std::unique_ptr<BaseScene>>& GetSceneQue();

	template<typename T>
	static void SetFactory();

private:
	static void NextScene();

#ifdef DEBUG_FEATURES_ENABLE
public:
	static void SetProfiler(Reference<TimestampProfiler> profiler_);
	static void DebugGui();
#endif // _DEBUG

private:
	/// <summary>
	/// シーンスタック
	/// </summary>
	std::deque<std::unique_ptr<BaseScene>> sceneQue;
	std::unique_ptr<BaseSceneFactory> factory;

	enum class SceneStatus {
		NANE,
		DEFAULT,
		CHANGE
	} sceneStatus;

	enum class SceneChangeType {
		CHANGE,
		STACK,
		POP,
	};

	struct SceneChangeInfo {
		SceneChangeType type;
		std::unique_ptr<BaseScene> next;
		bool isStopLoad;
		TimedCall<void(void)> endCall;
	} sceneChangeInfo;

#ifdef DEBUG_FEATURES_ENABLE
	Reference<TimestampProfiler> profiler;
#endif // _DEBUG
};

template<typename T>
inline void SceneManager::SetFactory() {
	auto& instance = GetInstance();
	instance.factory = std::make_unique<T>();
}
