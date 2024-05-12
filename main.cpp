#include "Engine/WinApp.h"
#include "Engine/Math/Camera3D.h"

#include "Engine/GameObject/GameObject.h"
#include "Engine/GameObject/PolygonMesh/PolygonMeshManager/PolygonMeshManager.h"


#include "Engine/DirectX/DirectXResourceObject/ConstantBuffer/ConstantBuffer.h"
#include "Engine/Math/Color.h"
#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"
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

	PolygonMeshManager::LoadPolygonMesh("./Engine/Resources/", "Triangle.obj");

	GameObject triangle = PolygonMeshManager::GetPolygonMesh("Triangle.obj");
	ConstantBuffer<DirectionalLightData> light{ {Color{1.0f,1.0f,1.0f,1.0f}, -Vec3::kBasisY, 1.0f} };

	while (!WinApp::IsEndApp()) {
		WinApp::BeginFrame();

		DirectXCommand::GetCommandList()->SetGraphicsRootConstantBufferView(2, light.get_resource()->GetGPUVirtualAddress());

		triangle.begin_rendering();
		triangle.draw();

		WinApp::EndFrame();
	}

	WinApp::Finalize();
}