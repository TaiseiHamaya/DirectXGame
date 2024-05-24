#include "Engine/WinApp.h"
#include "Engine/Math/Camera3D.h"
#include "Engine/Math/Camera2D.h"

#include "Engine/GameObject/GameObject.h"
#include "Engine/GameObject/PolygonMesh/PolygonMeshManager/PolygonMeshManager.h"
#include "Engine/GameObject/SpriteObject.h"

#include "externals/imgui/imgui.h"

#include "Engine/DirectX/DirectXResourceObject/ConstantBuffer/ConstantBuffer.h"
#include "Engine/Math/Color.h"
#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/DirectX/DirectXResourceObject/Texture/Texture.h"
#include "Engine/DirectX/DirectXResourceObject/Texture/TextureManager/TextureManager.h"

struct DirectionalLightData {
	Color color; // 色
	Vector3 direction; // 向き
	float intensity; // 輝度
};

// クライアント領域サイズ
const int32_t kClientWidth = 1280;
const int32_t kClientHight = 720;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WinApp::Initialize("CG2", kClientWidth, kClientHight);
	Camera3D::Initialize();
	Camera2D::Initialize();

	PolygonMeshManager::LoadPolygonMesh("./Engine/Resources/", "suzanne.obj");
	TextureManager::RegisterLoadQue("./Engine/Resources/", "uvChecker.png");
	TextureManager::RegisterLoadQue("./Engine/Resources/", "monsterBall.png");
	TextureManager::RegisterLoadQue("./Engine/Resources/", "uvChecker.png");
	TextureManager::LoadImperative();
	TextureManager::WaitEndExecute();

	GameObject triangle{ PolygonMeshManager::GetPolygonMesh("suzanne.obj") };
	ConstantBuffer<DirectionalLightData> light{ {Color{1.0f,1.0f,1.0f,1.0f}, -Vec3::kBasisY, 1.0f} };
	std::weak_ptr<Texture> texture = TextureManager::GetTexture("uvChecker.png");
	SpriteObject sprite{ TextureManager::GetTexture("uvChecker.png") };

	while (!WinApp::IsEndApp()) {
		WinApp::BeginFrame();

		DirectXCommand::GetCommandList()->SetGraphicsRootConstantBufferView(3, light.get_resource()->GetGPUVirtualAddress());

#ifdef _DEBUG
		Camera3D::DebugGUI();
		Camera2D::Begin();
		Camera2D::DebugGUI();
		Camera2D::CameraUpdate();

		// PolygonMesh
		ImGui::SetNextWindowSize(ImVec2{ 330,300 }, ImGuiCond_Once);
		ImGui::SetNextWindowPos(ImVec2{ 900, 50 }, ImGuiCond_Once);
		ImGui::Begin("Grid", nullptr, ImGuiWindowFlags_NoSavedSettings);
		triangle.debug_gui();
		ImGui::End();

		// Light
		ImGui::SetNextWindowSize(ImVec2{ 330,125 }, ImGuiCond_Once);
		ImGui::SetNextWindowPos(ImVec2{ 50, 370 }, ImGuiCond_Once);
		ImGui::Begin("Light", nullptr, ImGuiWindowFlags_NoSavedSettings);
		light.get_data()->color.debug_gui();
		Vector3 rotate = Vec3::kZero;
		ImGui::Text(std::format("X : {:.3}, Y : {:.3}, Z : {:.3}", light.get_data()->direction.x, light.get_data()->direction.y, light.get_data()->direction.z).c_str());
		if (ImGui::DragFloat3("DirectionRotate", &rotate.x, 0.02f)) {
			light.get_data()->direction = Transform3D::Homogeneous(light.get_data()->direction, Quaternion{ rotate, rotate.length() }.to_matrix());
		}
		ImGui::DragFloat("Intensity", &light.get_data()->intensity, 0.01f, 0.0f, (std::numeric_limits<float>::max)());
		ImGui::End();

		// Sprite
		ImGui::SetNextWindowSize(ImVec2{ 330,275 }, ImGuiCond_Once);
		ImGui::SetNextWindowPos(ImVec2{ 900, 370 }, ImGuiCond_Once);
		ImGui::Begin("Sprite", nullptr, ImGuiWindowFlags_NoSavedSettings);
		sprite.debug_gui();
		ImGui::End();
#endif // _DEBUG

		Camera3D::CameraUpdate();
		
		sprite.begin_rendering();
		triangle.begin_rendering();

		texture.lock()->set_command();
		
		triangle.draw();
		sprite.draw();

		WinApp::EndFrame();
	}

	WinApp::Finalize();
}