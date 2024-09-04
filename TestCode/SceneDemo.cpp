#include "SceneDemo.h"

#include "Engine/Game/Managers/SceneManager/SceneManager.h"
#include "Engine/Utility/Utility.h"

void SceneDemo::initialize() {
}

void SceneDemo::begin() {
}

void SceneDemo::update() {
}

void SceneDemo::begin_rendering() {
}

void SceneDemo::late_update() {
}

void SceneDemo::draw() const {
}

#ifdef _DEBUG

#include <externals/imgui/imgui.h>
void SceneDemo::debug_update() {
	ImGui::Begin("DemoScene");
	if (ImGui::Button("StackScene")) {
		SceneManager::SetSceneChange(CreateUnique<SceneDemo>(), true);
	}
	if (ImGui::Button("ChangeScene")) {
		SceneManager::SetSceneChange(CreateUnique<SceneDemo>(), false);
	}
	if (ImGui::Button("PopScene")) {
		SceneManager::PopScene();
	}
	ImGui::End();
}
#endif // _DEBUG
