#include "SceneManager.h"

#include "Engine/Game/Scene/BaseScene.h"
#include "Engine/Utility/Utility.h"

#include <cassert>
#include <algorithm>
#include <format>

SceneManager& SceneManager::GetInstance() {
	static SceneManager instance{};
	return instance;
}

void SceneManager::Initialize(std::unique_ptr<BaseScene>&& initScene) {
	SceneManager& instance = GetInstance();
	assert(instance.sceneQue.empty());
	Log(std::format("[SceneManager] Initialize SceneManager. Address-\'{}\'.\n", (void*)initScene.get()));
	instance.sceneQue.emplace_back(nullptr);
	instance.sceneQue.emplace_back(std::move(initScene));
	instance.sceneStatus = SceneStatus::DEFAULT;
}

void SceneManager::Finalize() {
	SceneManager& instance = GetInstance();
	instance.sceneQue.clear();
}

void SceneManager::Begin() {
	SceneManager& instance = GetInstance();
	if (instance.sceneStatus != SceneStatus::DEFAULT) {
		if (instance.sceneChangeInfo.isStackInitial) {
			instance.sceneQue.emplace_back(std::move(instance.sceneChangeInfo.next));
		}
		else {
			instance.sceneQue.back() = std::move(instance.sceneChangeInfo.next);
		}
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

void SceneManager::SetSceneChange(std::unique_ptr<BaseScene>&& nextScenePtr, bool isStackInitialScene) {
	SceneManager& instance = GetInstance();
	Log(std::format("[SceneManager] Set scene change. Internal scene address-\'{}\' Terminal scene address-\'{}\'. Is stack : {:s}.\n",
		(void*)instance.sceneQue.back().get(),
		(void*)nextScenePtr.get(), 
		isStackInitialScene
	));
	assert(instance.sceneStatus == SceneStatus::DEFAULT);
	//instance.sceneStatus = SceneStatus::CHANGE_BEFORE;
	instance.sceneStatus = SceneStatus::CHANGE_INSTANCE;
	instance.sceneChangeInfo.isStackInitial = isStackInitialScene;
	instance.sceneChangeInfo.next = std::move(nextScenePtr);
}

void SceneManager::PopScene() {
	SceneManager& instance = GetInstance();
	assert(instance.sceneStatus == SceneStatus::DEFAULT);
	assert(instance.sceneQue.size() >= 2);
	//instance.sceneStatus = SceneStatus::CHANGE_BEFORE;
	instance.sceneStatus = SceneStatus::CHANGE_INSTANCE;
	instance.sceneChangeInfo.isStackInitial = false;

	std::iter_swap(instance.sceneQue.rbegin(), instance.sceneQue.rbegin() + 1);

	instance.sceneChangeInfo.next = std::move(instance.sceneQue.back());
	instance.sceneQue.pop_back();

	Log(std::format("[SceneManager] Pop scene. Pop scene address-\'{}\' Next scene address-\'{}\'.\n",
		(void*)instance.sceneQue.back().get(),
		(void*)instance.sceneChangeInfo.next.get()
	));
}

bool SceneManager::IsEndProgram() {
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
