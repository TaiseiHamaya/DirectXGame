#include "SceneManager.h"

#include <algorithm>

#include "Engine/Application/Output.h"
#include "Engine/Assets/BackgroundLoader/BackgroundLoader.h"
#include "Engine/Runtime/Scene/BaseScene.h"
#include "Engine/Runtime/Scene/BaseSceneFactory.h"

#ifdef DEBUG_FEATURES_ENABLE
#include "Engine/Debug/Profiler/TimestampProfiler.h"
#endif // _DEBUG

SceneManager& SceneManager::GetInstance() noexcept {
	static SceneManager instance{};
	return instance;
}

void SceneManager::Initialize() {
	SceneManager& instance = GetInstance();
	CriticalIf(!instance.sceneQue.empty(), "Scene manager is already initalized.");

	// 最初にnullptrをemplace_backする
	instance.sceneQue.emplace_back(nullptr);
	auto initScene = instance.factory->initialize_scene();
	CriticalIf(!initScene, "The inital scene crated is nullptr.");

	initScene->load();
	BackgroundLoader::WaitEndExecute();
	initScene->initialize();

	Infomation("Initialize SceneManager. Address-\'{}\'.", (void*)initScene.get());

	instance.sceneQue.emplace_back(std::move(initScene));
	instance.sceneStatus = SceneStatus::DEFAULT;
	instance.sceneChangeInfo.endCall = {
		&SceneManager::NextScene ,0
	};
}

void SceneManager::Finalize() noexcept {
	SceneManager& instance = GetInstance();
	instance.sceneQue.clear();
}

void SceneManager::Begin() {
	SceneManager& instance = GetInstance();
	if (instance.sceneStatus != SceneStatus::DEFAULT) {
		instance.sceneChangeInfo.endCall.update();
	}
}

void SceneManager::Update() {
#ifdef DEBUG_FEATURES_ENABLE
	Reference<TimestampProfiler>& profiler = GetInstance().profiler;
#endif // _DEBUG
	BaseScene* nowScene = GetInstance().sceneQue.back().get();
#ifdef DEBUG_FEATURES_ENABLE
	if (profiler) {
		profiler->timestamp("Begin");
	}
#endif // _DEBUG
	nowScene->begin();
#ifdef DEBUG_FEATURES_ENABLE
	if (profiler) {
		profiler->timestamp("ImGui");
	}
	nowScene->debug_update();
	if (profiler) {
		profiler->timestamp("Update");
	}
#endif // _DEBUG
	nowScene->update();
#ifdef DEBUG_FEATURES_ENABLE
	if (profiler) {
		profiler->timestamp("BeginRendering");
	}
#endif // _DEBUG
	nowScene->begin_rendering();
#ifdef DEBUG_FEATURES_ENABLE
	if (profiler) {
		profiler->timestamp("LateUpdate");
	}
#endif // _DEBUG
	nowScene->late_update();
}

void SceneManager::Draw() {
	GetInstance().sceneQue.back()->draw();
}

//void SceneManager::Debug() {
//	sceneQue.back()->DebugDraw();
//}

void SceneManager::SetSceneChange(int32_t next, float interval, bool isStackInitialScene, bool isStopLoad) {
	SceneManager& instance = GetInstance();
	// シーンがDefault状態でないと遷移させない
	if (instance.sceneStatus != SceneStatus::DEFAULT) {
		return;
	}
	auto nextScenePtr = instance.factory->create_scene(next);
	Infomation("Set scene change. Internal scene address-\'{}\', Terminal scene address-\'{}\', Interval-{}, Stack-{:s}, Stop load-{:s},",
		(void*)instance.sceneQue.back().get(),
		(void*)nextScenePtr.get(),
		interval,
		isStackInitialScene,
		isStopLoad
	);
	// この時点でロード関数を呼び出し
	nextScenePtr->load();
	// 遷移状態にする
	instance.sceneStatus = SceneStatus::CHANGE;
	// 各種記録
	instance.sceneChangeInfo.type = isStackInitialScene ? SceneChangeType::STACK : SceneChangeType::CHANGE;
	instance.sceneChangeInfo.next = std::move(nextScenePtr);
	instance.sceneChangeInfo.isStopLoad = isStopLoad;
	instance.sceneChangeInfo.endCall.restart(interval);
}

void SceneManager::PopScene(float interval) {
	SceneManager& instance = GetInstance();
	// シーンがDefault状態でないと遷移させない
	if (instance.sceneStatus != SceneStatus::DEFAULT) {
		return;
	}
	// スタックしたシーン数が2未満の場合はおかしいので停止させる
	//ErrorIf(instance.sceneQue.size() >= 2);
	// 遷移状態にする
	instance.sceneStatus = SceneStatus::CHANGE;
	// Popするときはスタックさせない
	instance.sceneChangeInfo.type = SceneChangeType::POP;
	instance.sceneChangeInfo.isStopLoad = false;
	instance.sceneChangeInfo.endCall.restart(interval);
	// Pop後のシーンを取り出し
	std::iter_swap(instance.sceneQue.rbegin(), instance.sceneQue.rbegin() + 1);
	instance.sceneChangeInfo.next = std::move(instance.sceneQue.back());
	// nullptrになった要素を削除
	instance.sceneQue.pop_back();

	Infomation("Pop scene. Pop scene address-\'{}\', Next scene address-\'{}\', Interval-{},",
		(void*)instance.sceneQue.back().get(),
		(void*)instance.sceneChangeInfo.next.get(),
		interval
	);
}

bool SceneManager::IsEndProgram() noexcept {
	return GetInstance().sceneQue.back() == nullptr;
}

const std::deque<std::unique_ptr<BaseScene>>& SceneManager::GetSceneQue() {
	return GetInstance().sceneQue;
}

void SceneManager::NextScene() {
	SceneManager& instance = GetInstance();
	// シーンの切り替え
	if (instance.sceneChangeInfo.type == SceneChangeType::STACK) {
		// スタックする場合emplace_back
		instance.sceneQue.emplace_back(std::move(instance.sceneChangeInfo.next));
	}
	else {
		auto& currentScene = instance.sceneQue.back();
		// finalize関数の呼び出し
		currentScene->finalize();
		// スタックしないのでbackにmove
		currentScene = std::move(instance.sceneChangeInfo.next);
	}
	// ロードを止める
	if (instance.sceneChangeInfo.isStopLoad) {
		BackgroundLoader::WaitEndExecute();
	}
	// initialize関数の呼び出し
	auto& newScene = instance.sceneQue.back();
	if (newScene) {
		if (instance.sceneChangeInfo.type != SceneChangeType::POP) {
			newScene->initialize();
		}
		else {
			newScene->popped();
		}
	}
	// 遷移状態を解除
	instance.sceneStatus = SceneStatus::DEFAULT;
}

#ifdef DEBUG_FEATURES_ENABLE

#include <imgui.h>
#include <format>

void SceneManager::SetProfiler(Reference<TimestampProfiler> profiler_) {
	GetInstance().profiler = profiler_;
}

void SceneManager::DebugGui() {
	auto& instance = GetInstance();

	ImGui::Begin("SceneManager");
	ImGui::Text(std::format("SceneAddress- \'{:016}\'", (void*)instance.sceneQue.back().get()).c_str());
	ImGui::Text(std::format("SceneCount :  {}", instance.sceneQue.size() - 1).c_str());
	ImGui::Text(std::format("IsSceneChange : {:s}", instance.sceneChangeInfo.next != nullptr).c_str());
	ImGui::End();
}
#endif // _DEBUG