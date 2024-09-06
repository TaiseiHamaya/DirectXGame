#include "SceneDemo.h"

#include "Engine/Game/Camera/Camera3D.h"
#include "Engine/Game/GameObject/GameObject.h"
#include "Engine/Game/Hierarchy/Hierarchy.h"
#include "Engine/Game/Managers/PolygonMeshManager/PolygonMeshManager.h"
#include "Engine/Game/Managers/SceneManager/SceneManager.h"
#include "Engine/Render/RenderPathManager/RenderPathManager.h"
#include "Engine/Utility/Utility.h"

SceneDemo::SceneDemo() = default;

SceneDemo::~SceneDemo() = default;

void SceneDemo::load() {
	PolygonMeshManager::RegisterLoadQue("./Engine/Resources/", "bunny.obj");
}

void SceneDemo::initialize() {
	camera3D = CreateUnique<Camera3D>();
	camera3D->initialize();
	camera3D->set_transform({
		CVector3::BASIS,
		Quaternion::EulerDegree(45,0,0),
		{0,10,-10}
		});
	parent = CreateUnique<GameObject>();
	parent->reset_object("bunny.obj");
	child = CreateUnique<GameObject>();
	child->reset_object("bunny.obj");
	child->set_parent(*parent);
}

void SceneDemo::finalize() {
}

void SceneDemo::begin() {
}

void SceneDemo::update() {
	camera3D->update();
}

void SceneDemo::begin_rendering() {
	camera3D->update_matrix();
	parent->begin_rendering(*camera3D);
	child->begin_rendering(*camera3D);
}

void SceneDemo::late_update() {
}

void SceneDemo::draw() const {
	RenderPathManager::BeginFrame();
	parent->draw();
	child->draw();
	RenderPathManager::Next();
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

	ImGui::Begin("Camera3D");
	camera3D->debug_gui();
	ImGui::End();

	ImGui::Begin("Parent");
	parent->debug_gui();
	ImGui::End();
	
	ImGui::Begin("Child");
	child->debug_gui();
	ImGui::End();

}
#endif // _DEBUG
