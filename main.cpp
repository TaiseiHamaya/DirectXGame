#include "Engine/Math/Camera2D.h"
#include "Engine/Math/Camera3D.h"
#include "Engine/WinApp.h"

#include <cstdint>

#include "Engine/GameObject/GameObject.h"
#include "Engine/GameObject/PolygonMesh/PolygonMeshManager/PolygonMeshManager.h"
#include "Engine/Utility/BackgroundLoader/BackgroundLoader.h"

#include "externals/imgui/imgui.h"

#include "Engine/DirectX/DirectXCore.h"
#include <minwindef.h>
#include <winnt.h>
#include "Engine/Utility/ImGuiLoadManager/ImGuiLoadManager.h"
#include <format>
#include <unordered_set>

// クライアント領域サイズ
const std::int32_t kClientWidth = 1280;
const std::int32_t kClientHight = 720;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WinApp::Initialize("CG2", kClientWidth, kClientHight);
	Camera3D::Initialize();
	Camera2D::Initialize();

	PolygonMeshManager::RegisterLoadQue("./Engine/Resources", "Sphere.obj");
	BackgroundLoader::WaitEndExecute();

	std::vector<GameObject> objects;
	std::vector<std::string> objectNames;
	objects.emplace_back("Sphere.obj");
	objectNames.emplace_back("Sphere");
	std::unordered_multiset<std::string> objectList;
	objectList.emplace("Sphere");

	std::string selectMesh;
	char name[1024]{};

	while (!WinApp::IsEndApp()) {
		WinApp::BeginFrame();

#ifdef _DEBUG
		DirectXCore::ShowDebugTools();
		DirectXCore::ShowGrid();

		ImGuiID objectDock = ImGui::GetID("ObjectDock");

		ImGui::SetNextWindowSize(ImVec2{ 330,100 }, ImGuiCond_Once);
		ImGui::SetNextWindowPos(ImVec2{ 50, 250 }, ImGuiCond_Once);
		ImGui::Begin("Main", nullptr, ImGuiWindowFlags_NoSavedSettings);
		PolygonMeshManager::MeshListGui(selectMesh);
		ImGui::InputText("Name", const_cast<char*>(name), 1024);
		if (ImGui::Button("CreateObject") && !selectMesh.empty() && name[0] != '\0') {
			if (!objectList.contains(name)) {
				objects.emplace_back(selectMesh);
				objectNames.emplace_back(name);
				objectList.emplace(name);
			}
			else {
				objects.emplace_back(selectMesh);
				std::string test = std::format("{}-{}", name, objectList.count(name));
				objectNames.push_back(test);
				objectList.insert(test);
				objectList.insert(name);
			}
		}
		ImGui::End();

		ImGuiLoadManager::ShowGUI();

		for (int i = 0; i < objects.size(); ++i) {
			ImGui::SetNextWindowDockID(objectDock, 0);
			ImGui::SetNextWindowSize(ImVec2{ 330,365 }, ImGuiCond_Once);
			ImGui::SetNextWindowPos(ImVec2{ 900, 50 }, ImGuiCond_Once);
			ImGui::Begin(objectNames[i].c_str(), nullptr, ImGuiWindowFlags_NoSavedSettings);
			objects[i].debug_gui();
			ImGui::End();
		}

#endif // _DEBUG

		Camera2D::Begin();
		Camera2D::CameraUpdate();
		Camera3D::CameraUpdate();

		for (int i = 0; i < objects.size(); ++i) {
			objects[i].begin_rendering();
		}

		for (int i = 0; i < objects.size(); ++i) {
			objects[i].draw();
		}

		WinApp::EndFrame();
	}

	WinApp::Finalize();
}