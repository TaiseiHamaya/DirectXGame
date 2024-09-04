#pragma once

class BaseScene;

#include <memory>
#include <deque>

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
	static SceneManager& GetInstance();

public:
	static void Initialize(std::unique_ptr<BaseScene>&& initScene);
	static void Finalize();

	static void Begin();
	static void Update();
	static void Draw();
	//static void Debug();
	static void SetSceneChange(
		std::unique_ptr<BaseScene>&& nextScenePtr,
		bool isStackInitialScene_ = false
	);
	static void PopScene();
	static bool IsEndProgram();

	static const std::deque<std::unique_ptr<BaseScene>>& GetSceneQue();

#ifdef _DEBUG
	static void DebugGui();
#endif // _DEBUG

private:
	static void NextScene();

private:
	/// <summary>
	/// シーンスタック
	/// </summary>
	std::deque<std::unique_ptr<BaseScene>> sceneQue;

	enum class SceneStatus {
		NANE,
		DEFAULT,
		CHANGE_BEFORE,
		CHANGE_AFTER,
		CHANGE_INSTANCE, // for debug
	} sceneStatus;

	struct SceneChangeInfo {
		bool isStackInitial;
		std::unique_ptr<BaseScene> next;
	} sceneChangeInfo;
};
