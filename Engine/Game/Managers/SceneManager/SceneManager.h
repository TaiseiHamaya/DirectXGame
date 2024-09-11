#pragma once

class BaseScene;

#include <memory>
#include <deque>

#include "Engine/Utility/TimedCall/TimedCall.h"

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
	static void Initialize(std::unique_ptr<BaseScene>&& initScene);
	static void Finalize() noexcept;

	static void Begin();
	static void Update();
	static void Draw();
	//static void Debug();
	static void SetSceneChange(
		std::unique_ptr<BaseScene>&& nextScenePtr,
		float interval,
		bool isStackInitialScene_ = false,
		bool isStopLoad = true
	);
	static void PopScene(float interval);
	static bool IsEndProgram() noexcept;

	static const std::deque<std::unique_ptr<BaseScene>>& GetSceneQue();

private:
	static void NextScene();

#ifdef _DEBUG
public:
	static void DebugGui();
#endif // _DEBUG

private:
	/// <summary>
	/// シーンスタック
	/// </summary>
	std::deque<std::unique_ptr<BaseScene>> sceneQue;

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
};
