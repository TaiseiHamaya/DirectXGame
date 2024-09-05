#include "SceneManager.h"

#include "Engine/Game/Scene/BaseScene.h"
#include "Engine/Utility/Utility.h"
#include "Engine/Utility/BackgroundLoader/BackgroundLoader.h"

#include <cassert>
#include <algorithm>
#include <format>

SceneManager& SceneManager::GetInstance() noexcept {
	static SceneManager instance{};
	return instance;
}

void SceneManager::Initialize(std::unique_ptr<BaseScene>&& initScene) {
	assert(initScene);
	initScene->load();
	BackgroundLoader::WaitEndExecute();
	initScene->initialize();

	SceneManager& instance = GetInstance();
	assert(instance.sceneQue.empty());
	Log(std::format("[SceneManager] Initialize SceneManager. Address-\'{}\'.\n", (void*)initScene.get()));
	// 最初にnullptrをemplace_backする
	instance.sceneQue.emplace_back(nullptr);
	instance.sceneQue.emplace_back(std::move(initScene));
	instance.sceneStatus = SceneStatus::DEFAULT;
}

void SceneManager::Finalize() noexcept {
	SceneManager& instance = GetInstance();
	instance.sceneQue.clear();
}

void SceneManager::Begin() {
	SceneManager& instance = GetInstance();
	if (instance.sceneStatus != SceneStatus::DEFAULT) {
		// initialize関数の呼び出し
		if (instance.sceneChangeInfo.next && instance.sceneChangeInfo.changeType != SceneChangeType::POP) {
			instance.sceneChangeInfo.next->initialize();
		}
		// finalize関数の呼び出し
		instance.sceneQue.back()->finalize();
		// シーンの切り替え
		if (instance.sceneChangeInfo.changeType == SceneChangeType::STACK) {
			// スタックする場合emplace_back
			instance.sceneQue.emplace_back(std::move(instance.sceneChangeInfo.next));
		}
		else {
			// スタックしないのでbackにmove
			instance.sceneQue.back() = std::move(instance.sceneChangeInfo.next);
		}
		// ロードを止める
		if (instance.sceneChangeInfo.isStopLoad) {
			BackgroundLoader::WaitEndExecute();
		}
		// 遷移状態を解除
		instance.sceneStatus = SceneStatus::DEFAULT;
	}
}

void SceneManager::Update() {
	BaseScene* nowScene = GetInstance().sceneQue.back().get();
	nowScene->begin();
	nowScene->update();
#ifdef _DEBUG
	nowScene->debug_update();
#endif // _DEBUG
	nowScene->begin_rendering();
	nowScene->late_update();
}

void SceneManager::Draw() {
	GetInstance().sceneQue.back()->draw();
}

//void SceneManager::Debug() {
//	sceneQue.back()->DebugDraw();
//}

void SceneManager::SetSceneChange(std::unique_ptr<BaseScene>&& nextScenePtr, bool isStackInitialScene, bool isStopLoad) {
	assert(nextScenePtr);
	SceneManager& instance = GetInstance();
	Log(std::format("[SceneManager] Set scene change. Internal scene address-\'{}\' Terminal scene address-\'{}\'. Is stack : {:s}. Is stop load : {:s}.\n",
		(void*)instance.sceneQue.back().get(),
		(void*)nextScenePtr.get(),
		isStackInitialScene,
		isStopLoad
	));
	// シーンがDefault状態でないと遷移させない
	assert(instance.sceneStatus == SceneStatus::DEFAULT);
	// この時点でロード関数を呼び出し
	nextScenePtr->load();
	// 遷移状態にする
	//instance.sceneStatus = SceneStatus::CHANGE_BEFORE;
	instance.sceneStatus = SceneStatus::CHANGE_INSTANCE;
	// 各種記録
	instance.sceneChangeInfo.changeType = isStackInitialScene ? SceneChangeType::STACK : SceneChangeType::CHANGE;
	instance.sceneChangeInfo.next = std::move(nextScenePtr);
	instance.sceneChangeInfo.isStopLoad = isStopLoad;
}

void SceneManager::PopScene() {
	SceneManager& instance = GetInstance();
	// シーンがDefault状態でないと遷移させない
	assert(instance.sceneStatus == SceneStatus::DEFAULT);
	// スタックしたシーン数が2未満の場合はおかしいので停止させる
	assert(instance.sceneQue.size() >= 2);
	// 遷移状態にする
	//instance.sceneStatus = SceneStatus::CHANGE_BEFORE;
	instance.sceneStatus = SceneStatus::CHANGE_INSTANCE;
	// Popするときはスタックさせない
	instance.sceneChangeInfo.changeType = SceneChangeType::POP;
	instance.sceneChangeInfo.isStopLoad = false;
	// Pop後のシーンを取り出し
	std::iter_swap(instance.sceneQue.rbegin(), instance.sceneQue.rbegin() + 1);
	instance.sceneChangeInfo.next = std::move(instance.sceneQue.back());
	// nullptrになった要素を削除
	instance.sceneQue.pop_back();

	Log(std::format("[SceneManager] Pop scene. Pop scene address-\'{}\' Next scene address-\'{}\'.\n",
		(void*)instance.sceneQue.back().get(),
		(void*)instance.sceneChangeInfo.next.get()
	));
}

bool SceneManager::IsEndProgram() noexcept {
	return GetInstance().sceneQue.back() == nullptr;
}

const std::deque<std::unique_ptr<BaseScene>>& SceneManager::GetSceneQue() {
	return GetInstance().sceneQue;
}

#ifdef _DEBUG

#include <externals/imgui/imgui.h>

void SceneManager::DebugGui() {
	auto& instance = GetInstance();

	ImGui::Begin("SceneManager");
	ImGui::Text(std::format("SceneAddress- \'{}\'", (void*)instance.sceneQue.back().get()).c_str());
	ImGui::Text(std::format("SceneCount :  {}", instance.sceneQue.size() - 1).c_str());
	ImGui::Text(std::format("IsSceneChange : {:s}", instance.sceneChangeInfo.next != nullptr).c_str());
	ImGui::End();
}
#endif // _DEBUG

void SceneManager::NextScene() {
	SceneManager& instance = GetInstance();
	//instance.sceneStatus = SceneStatus::CHANGE_AFTER;

	instance.sceneQue.back() = std::move(instance.sceneChangeInfo.next);
}
