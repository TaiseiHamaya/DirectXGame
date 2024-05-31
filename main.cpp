#include "Engine/Math/Camera2D.h"
#include "Engine/Math/Camera3D.h"
#include "Engine/WinApp.h"

#include <cstdint>
#include <format>

#include "Engine/GameObject/GameObject.h"
#include "Engine/GameObject/PolygonMesh/PolygonMeshManager/PolygonMeshManager.h"
#include "Engine/Utility/BackgroundLoader/BackgroundLoader.h"
#include "Engine/GameObject/SpriteObject.h"

#include "externals/imgui/imgui.h"

#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/DirectX/DirectXResourceObject/ConstantBuffer/ConstantBuffer.h"
#include "Engine/DirectX/DirectXResourceObject/Texture/Texture.h"
#include "Engine/DirectX/DirectXResourceObject/Texture/TextureManager/TextureManager.h"
#include "Engine/GameObject/Transform3D/Transform3D.h"
#include "Engine/Math/Color.h"
#include "Engine/Math/Quaternion.h"
#include "Engine/Math/Vector3.h"
#include <limits>
#include <memory>
#include <minwindef.h>
#include <winnt.h>

struct DirectionalLightData {
	Color color; // 色
	Vector3 direction; // 向き
	float intensity; // 輝度
};

// クライアント領域サイズ
const std::int32_t kClientWidth = 1280;
const std::int32_t kClientHight = 720;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WinApp::Initialize("CG2", kClientWidth, kClientHight);
	Camera3D::Initialize();
	Camera2D::Initialize();

	TextureManager::RegisterLoadQue("./Engine/Resources/", "uvChecker.png");
	PolygonMeshManager::RegisterLoadQue("./Engine/Resources/", "Grid.obj");
	TextureManager::RegisterLoadQue("./Engine/Resources/", "monsterBall.png");
	TextureManager::RegisterLoadQue("./Engine/Resources/", "Grid.png");
	TextureManager::RegisterLoadQue("./Engine/Resources/", "monsterBall.png");
	PolygonMeshManager::RegisterLoadQue("./Engine/Resources/", "bunny.obj");
	PolygonMeshManager::RegisterLoadQue("./Engine/Resources/", "teapot.obj");
	BackgroundLoader::LoadImperative();
	BackgroundLoader::WaitEndExecute();

	GameObject grid{ PolygonMeshManager::GetPolygonMesh("Grid.obj") };
	GameObject bunny{ PolygonMeshManager::GetPolygonMesh("bunny.obj") };
	GameObject teapot{ PolygonMeshManager::GetPolygonMesh("teapot.obj") };
	ConstantBuffer<DirectionalLightData> light{ {Color{1.0f,1.0f,1.0f,1.0f}, -Vec3::kBasisY, 1.0f} };
	std::weak_ptr<Texture> texture = TextureManager::GetTexture("Grid.png");
	SpriteObject sprite{ TextureManager::GetTexture("uvChecker.png") };

	while (!WinApp::IsEndApp()) {
		WinApp::BeginFrame();

		DirectXCommand::GetCommandList()->SetGraphicsRootConstantBufferView(3, light.get_resource()->GetGPUVirtualAddress());

#ifdef _DEBUG
		ImGuiID objectDock = ImGui::GetID("ObjectDock");
		ImGuiID debugDock = ImGui::GetID("DebugDock");

		ImGui::SetNextWindowDockID(debugDock, 0);
		Camera3D::DebugGUI();
		Camera2D::Begin();
		ImGui::SetNextWindowDockID(debugDock, 0);
		Camera2D::DebugGUI();
		Camera2D::CameraUpdate();

		// Light
		ImGui::SetNextWindowDockID(debugDock, 0);
		ImGui::SetNextWindowSize(ImVec2{ 330,125 }, ImGuiCond_Once);
		ImGui::SetNextWindowPos(ImVec2{ 50, 370 }, ImGuiCond_Once);
		ImGui::Begin("Light", nullptr, ImGuiWindowFlags_NoSavedSettings);
		light.get_data()->color.debug_gui();
		Vector3 rotate = Vec3::kZero;
		ImGui::Text(std::format("X : {:.3}, Y : {:.3}, Z : {:.3}", light.get_data()->direction.x, light.get_data()->direction.y, light.get_data()->direction.z).c_str());
		if (ImGui::DragFloat3("DirectionRotate", &rotate.x, 0.02f)) {
			light.get_data()->direction = Transform3D::Homogeneous(light.get_data()->direction, Quaternion { rotate, rotate.length() }.to_matrix());
		}
		ImGui::DragFloat("Intensity", &light.get_data()->intensity, 0.01f, 0.0f, (std::numeric_limits<float>::max)());
		ImGui::End();


		// PolygonMesh
		ImGui::SetNextWindowDockID(objectDock, 0);
		ImGui::SetNextWindowSize(ImVec2{ 330,300 }, ImGuiCond_Once);
		ImGui::SetNextWindowPos(ImVec2{ 900, 50 }, ImGuiCond_Once);
		ImGui::Begin("Grid", nullptr, ImGuiWindowFlags_NoSavedSettings);
		grid.debug_gui();
		ImGui::End();

		// PolygonMesh2
		ImGui::SetNextWindowDockID(objectDock, 0);
		ImGui::SetNextWindowSize(ImVec2{ 330,300 }, ImGuiCond_Once);
		ImGui::SetNextWindowPos(ImVec2{ 900, 50 }, ImGuiCond_Once);
		ImGui::Begin("Bunny", nullptr, ImGuiWindowFlags_NoSavedSettings);
		bunny.debug_gui();
		ImGui::End();

		// PolygonMesh2
		ImGui::SetNextWindowDockID(objectDock, 0);
		ImGui::SetNextWindowSize(ImVec2{ 330,300 }, ImGuiCond_Once);
		ImGui::SetNextWindowPos(ImVec2{ 900, 50 }, ImGuiCond_Once);
		ImGui::Begin("teapot", nullptr, ImGuiWindowFlags_NoSavedSettings);
		teapot.debug_gui();
		ImGui::End();

		// Sprite
		ImGui::SetNextWindowDockID(objectDock, 0);
		ImGui::SetNextWindowSize(ImVec2{ 330,300 }, ImGuiCond_Once);
		ImGui::SetNextWindowPos(ImVec2{ 900, 50 }, ImGuiCond_Once);
		ImGui::Begin("Sprite", nullptr, ImGuiWindowFlags_NoSavedSettings);
		sprite.debug_gui();
		ImGui::End();

#endif // _DEBUG

		Camera3D::CameraUpdate();

		sprite.begin_rendering();
		grid.begin_rendering();
		bunny.begin_rendering();
		teapot.begin_rendering();

		texture.lock()->set_command();
		grid.draw();
		bunny.draw();
		sprite.draw();
		teapot.draw();

		WinApp::EndFrame();
	}

	WinApp::Finalize();
}