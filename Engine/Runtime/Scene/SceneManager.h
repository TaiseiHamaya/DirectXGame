#pragma once

class BaseScene;
class BaseSceneFactory;

#include <deque>
#include <memory>

#include <Library/Utility/Template/TimedCall.h>
#include <Library/Utility/Template/Reference.h>

#ifdef DEBUG_FEATURES_ENABLE
class TimestampProfiler;
#endif //DEBUG_FEATURES_ENABLE

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
	static void Setup(std::unique_ptr<BaseSceneFactory> factory_);
	static void Finalize() noexcept;

	static void Begin();
	static void Update();
	static void Draw();

	/// <summary>
	/// シーン変更命令の発行(既に発行済みの場合は何もしない)
	/// </summary>
	/// <param name="next">次シーンのシーン番号</param>
	/// <param name="interval">遷移にかかる時間</param>
	/// <param name="isStackInitialScene_">初期シーンをスタックするか</param>
	/// <param name="isStopLoad">ロード未完了時に停止するか</param>
	static void SetSceneChange(
		i32 next,
		r32 interval,
		bool isStackInitialScene_ = false,
		bool isStopLoad = true
	);
	static void PopScene(r32 interval);

public:
	static bool IsEndProgram() noexcept;

	static Reference<BaseScene> GetCurrentScene();
	static const std::deque<std::unique_ptr<BaseScene>>& GetSceneQue();

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
private:
	Reference<TimestampProfiler> profiler;
#endif // _DEBUG
};
