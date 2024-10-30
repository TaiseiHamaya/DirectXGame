#include "Engine/Rendering/DirectX/DirectXCore.h"

#include <memory>

#include <dxgidebug.h>
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

#ifdef _DEBUG
#include "Engine/Debug/ImGuiManager/ImGuiManager.h"
#endif // _DEBUG

#include "Engine/Debug/Output.h"
#include "Engine/Resources/BackgroundLoader/BackgroundLoader.h"
#include "Engine/Resources/Texture/TextureManager.h"
#include "Engine/Resources/PolygonMesh/PolygonMeshManager.h"
#include "Engine/Rendering/DirectX/DirectXDevice/DirectXDevice.h"
#include "Engine/Rendering/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/Rendering/DirectX/DirectXDescriptorHeap/RTVDescriptorHeap/RTVDescriptorHeap.h"
#include "Engine/Rendering/DirectX/DirectXDescriptorHeap/SRVDescriptorHeap/SRVDescriptorHeap.h"
#include "Engine/Rendering/DirectX/DirectXDescriptorHeap/DSVDescroptorHeap/DSVDescriptorHeap.h"
#include "Engine/Rendering/DirectX/DirectXSwapChain/DirectXSwapChain.h"
#include "Engine/Rendering/DirectX/ShaderCompiler/ShaderCompiler.h"
#include "Engine/Module/Render/RenderPathManager/RenderPathManager.h"
#include "Engine/Module/World/GameObject/GameObject.h"

#include "Library/Math/Vector3.h"
#include "Library/Math/Color.h"
#include "Library/Math/Quaternion.h"
#include "Engine/Module/World/Collision/CollisionManager.h"

#ifdef _DEBUG
#include "imgui.h"
#endif // _DEBUG

// ----------要修正----------
struct DirectionalLightData {
	Color color; // 色
	Vector3 direction; // 向き
	float intensity; // 輝度
};
// ----------要修正----------

static HRESULT hr;

DirectXCore::DirectXCore() = default;

DirectXCore::~DirectXCore() = default;

DirectXCore& DirectXCore::GetInstance() {
	static DirectXCore instance{};
	return instance;
}

void DirectXCore::Initialize() {
	// Debugの初期化(必ず一番初めに行う)
	Debug::Initialize();
	// Deviceの初期化
	DirectXDevice::Initialize();
	// InformationQueueの有効化
	Debug::InfoQueue();
	// Command関連の初期化
	DirectXCommand::Initialize();
	// RTVHeapの初期化
	RTVDescriptorHeap::Initialize();
	// SRVHeapの初期化
	SRVDescriptorHeap::Initialize();
	// DSVHeapの初期化
	DSVDescriptorHeap::Initialize();
	// シェーダーコンパイラ初期化
	ShaderCompiler::Initialize();
	// Swapchain初期化
	DirectXSwapChain::Initialize();
	// テクスチャマネージャの初期化
	TextureManager::Initialize();
	// バックグラウンドローダーの初期化
	BackgroundLoader::Initialize();
	// RenderPathManagerの初期化
	RenderPathManager::Initialize();

#ifdef _DEBUG
	ImGuiManager::Initialize();
#endif // _DEBUG
	// システム使用のオブジェクトをロード
	PolygonMeshManager::RegisterLoadQue("./EngineResources/Models/ErrorObject", "ErrorObject.obj");
	PolygonMeshManager::RegisterLoadQue("./EngineResources/Models/Frustum", "Frustum.obj");
	PolygonMeshManager::RegisterLoadQue("./EngineResources/Models", "Grid.obj");
	PolygonMeshManager::RegisterLoadQue("./EngineResources/Models/Camera", "CameraAxis.obj");
#ifdef _DEBUG
	CollisionManager::LoadDebugDrawMesh();
#endif // _DEBUG

	// 待機
	BackgroundLoader::WaitEndExecute();
	// Thisの初期化
	GetInstance().initialize();

	// オールコンプリート
	Console("[Engine] Complete create DirectXObjects\n");
}

void DirectXCore::BeginFrame() {
	// srvの設定
	SRVDescriptorHeap::SetDescriptorHeaps();

#ifdef _DEBUG
	ImGuiManager::BeginFrame();
#endif // _DEBUG
	//GetInstance().begin_frame();
}

void DirectXCore::EndFrame() {
	// レンダーパスが終わってないならおかしいので止める(デバッグ時のみ)
	assert(RenderPathManager::IsEnd());

#ifdef _DEBUG
	// 一番先にImGUIの処理
	ImGuiManager::EndFrame();
#endif // _DEBUG
	DirectXSwapChain::EndRenderTarget();
	// クローズしてエクスキュート
	DirectXCommand::GetInstance().close_and_kick();
	// スワップチェイン実行
	DirectXSwapChain::SwapScreen();
	// コマンド終了まで待つ
	DirectXCommand::GetInstance().wait_command();
	// コマンドリセット
	DirectXCommand::GetInstance().reset();
}

void DirectXCore::Finalize() {
	// ----------後で直す!!!----------
	GetInstance().gridMesh.reset();
	GetInstance().light.reset();
	// ----------後で直す!!!----------
	TextureManager::Finalize();
#ifdef _DEBUG
	ImGuiManager::Finalize();
#endif // _DEBUG
}

void DirectXCore::Set3DLight() {
	// ライトを設定しておく
	DirectXCommand::GetCommandList()->SetGraphicsRootConstantBufferView(3, GetInstance().light->get_resource()->GetGPUVirtualAddress());
}

#ifdef _DEBUG
void DirectXCore::ShowDebugTools() {
	GetInstance().show_debug_tools();
}
#endif // _DEBUG

void DirectXCore::ShowGrid() {
	GetInstance().gridMesh->begin_rendering();
	GetInstance().gridMesh->draw();
}

void DirectXCore::initialize() {
	// システム使用のオブジェクトを生成
	light = std::make_unique<ConstantBuffer<DirectionalLightData>>(DirectionalLightData{ Color{ 1.0f,1.0f,1.0f,1.0f }, -CVector3::BASIS_Y, 1.0f });
	gridMesh = std::make_unique<GameObject>("Grid.obj");
}

#ifdef _DEBUG

#include <format>

void DirectXCore::show_debug_tools() {
	ImGuiID debugDock = ImGui::GetID("DebugDock");

	// 3DカメラのImGui
	ImGui::SetNextWindowDockID(debugDock, ImGuiCond_FirstUseEver);
	//Camera3D::DebugGUI();

	// 2DカメラのImGui
	ImGui::SetNextWindowDockID(debugDock, ImGuiCond_FirstUseEver);
	//Camera2D::DebugGUI();

	// ライトのImGui
	//ImGui::SetNextWindowSize(ImVec2{ 330,165 }, ImGuiCond_Once);
	//ImGui::SetNextWindowPos(ImVec2{ 20, 20 }, ImGuiCond_Once);
	ImGui::SetNextWindowDockID(debugDock, ImGuiCond_FirstUseEver);
	ImGui::Begin("Light", nullptr);
	light->get_data()->color.debug_gui3();
	Vector3 rotate = CVector3::ZERO;
	ImGui::Text(std::format("X : {:.3}, Y : {:.3}, Z : {:.3}", light->get_data()->direction.x, light->get_data()->direction.y, light->get_data()->direction.z).c_str());
	if (ImGui::DragFloat3("DirectionRotate", &rotate.x, 1.0f, -180.0f, 180.0f)) {
		light->get_data()->direction = light->get_data()->direction * Quaternion::EulerDegree(rotate);
	}
	ImGui::Text("ResetDirection");
	if (ImGui::Button("X")) {
		light->get_data()->direction = CVector3::BASIS_X;
	}
	ImGui::SameLine();
	if (ImGui::Button("Y")) {
		light->get_data()->direction = CVector3::BASIS_Y;
	}
	ImGui::SameLine();
	if (ImGui::Button("Z")) {
		light->get_data()->direction = CVector3::BASIS_Z;
	}
	ImGui::SameLine();
	if (ImGui::Button("-X")) {
		light->get_data()->direction = -CVector3::BASIS_X;
	}
	ImGui::SameLine();
	if (ImGui::Button("-Y")) {
		light->get_data()->direction = -CVector3::BASIS_Y;
	}
	ImGui::SameLine();
	if (ImGui::Button("-Z")) {
		light->get_data()->direction = -CVector3::BASIS_Z;
	}
	ImGui::DragFloat("Intensity", &light->get_data()->intensity, 0.01f, 0.0f, (std::numeric_limits<float>::max)());
	ImGui::End();
}
#endif // _DEBUG

DirectXCore::Debug::~Debug() {
	// 全てが終了したあと、ReportLiveObjectsの実行
	Microsoft::WRL::ComPtr<IDXGIDebug1> debug;
	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(debug.GetAddressOf())))) {
		debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
	}
}

void DirectXCore::Debug::Initialize() {
	// ここでイニシャライズすることで、一番最後にデストラクタが呼び出される
	GetInstance();
	// DebugControllerを有効化
	Microsoft::WRL::ComPtr<ID3D12Debug1> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(debugController.GetAddressOf())))) {
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(true);
	}
}

void DirectXCore::Debug::InfoQueue() {
	// 警告、エラーを取得するよう設定
	Microsoft::WRL::ComPtr<ID3D12InfoQueue> infoQueue;
	if (SUCCEEDED(DirectXDevice::GetDevice()->QueryInterface(IID_PPV_ARGS(infoQueue.GetAddressOf())))) {
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
		D3D12_MESSAGE_ID denyIds[] = {
			// Windows11でのDXGIデバッグレイヤーとDX12デバッグレイヤーの相互作用バグによるエラーメッセージ
			// https://stackoverflow.com/questions/69805245/directx-12-application-is-crashing-in-windows-11
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
		};
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;
		// 適用
		infoQueue->PushStorageFilter(&filter);
	}
}

DirectXCore::Debug& DirectXCore::Debug::GetInstance() {
	static DirectXCore::Debug instance{};
	return instance;
}
