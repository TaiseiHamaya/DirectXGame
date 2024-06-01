#include "Engine/DirectX/DirectXCore.h"

#include <memory>
#include <format>

#include <dxgidebug.h>
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

#ifdef _DEBUG
#include "Engine/Utility/ImGuiManager/ImGuiManager.h"
#endif // _DEBUG

#include "Engine/WinApp.h"
#include "Engine/Utility/Utility.h"
#include "Engine/DirectX/DirectXDevice/DirectXDevice.h"
#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/DirectX/DirectXDescriptorHeap/RTVDescriptorHeap/RTVDescriptorHeap.h"
#include "Engine/DirectX/DirectXDescriptorHeap/SRVDescriptorHeap/SRVDescriptorHeap.h"
#include "Engine/DirectX/DirectXDescriptorHeap/DSVDescroptorHeap/DSVDescriptorHeap.h"
#include "Engine/DirectX/DirectXSwapChain/DirectXSwapChain.h"
#include "Engine/Utility/ShaderCompiler/ShaderCompiler.h"
#include "Engine/DirectX/PipelineState/PipelineState.h"
#include "Engine/DirectX/DirectXResourceObject/Texture/TextureManager/TextureManager.h"
#include "Engine/GameObject/PolygonMesh/PolygonMeshManager/PolygonMeshManager.h"
#include "Engine/Utility/BackgroundLoader/BackgroundLoader.h"
#include "Engine/GameObject/GameObject.h"

#include "Engine/Math/Camera2D.h"
#include "Engine/Math/Camera3D.h"

#include "Engine/Math/Vector3.h"
#include "Engine/Math/Color.h"
#include "Engine/Math/Quaternion.h"
#include "Engine/GameObject/Transform3D/Transform3D.h"
#include "externals/imgui/imgui.h"
struct DirectionalLightData {
	Color color; // 色
	Vector3 direction; // 向き
	float intensity; // 輝度
};

static HRESULT hr;

DirectXCore::DirectXCore()
	: viewPort(),
	scissorRect() {
};

DirectXCore::~DirectXCore() = default;

void DirectXCore::Initialize() {
	GetInstance();
	GetInstance().initialize();
}

void DirectXCore::BeginFrame() {
	GetInstance().begin_frame();
}

void DirectXCore::EndFrame() {
	GetInstance().end_frame();
}

void DirectXCore::Finalize() {
	// ----------後で直す!!!----------
	GetInstance().pipelineState.reset();
	GetInstance().gridMesh.reset();
	GetInstance().light.reset();
	// ----------後で直す!!!----------
#ifdef _DEBUG
	ImGuiManager::Finalize();
#endif // _DEBUG
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

DirectXCore& DirectXCore::GetInstance() {
	static std::unique_ptr<DirectXCore> instance{ new DirectXCore };
	return *instance;
}

void DirectXCore::initialize() {
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
	// Swapchain初期化
	DirectXSwapChain::Initialize();
	// シェーダーコンパイラ初期化
	ShaderCompiler::Initialize();
	// PSO生成
	pipelineState = std::make_unique<PipelineState>();
	pipelineState->initialize();

	TextureManager::Initialize();

	BackgroundLoader::Initialize();

#ifdef _DEBUG
	ImGuiManager::Initialize();
#endif // _DEBUG

	// ViewPort設定
	viewPort.Width = static_cast<FLOAT>(WinApp::GetClientWidth());
	viewPort.Height = static_cast<FLOAT>(WinApp::GetClientHight());
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	// シーザー矩形設定
	scissorRect.left = 0;
	scissorRect.right = static_cast<LONG>(WinApp::GetClientWidth());
	scissorRect.top = 0;
	scissorRect.bottom = static_cast<LONG>(WinApp::GetClientHight());

	// システム使用のオブジェクトとスプライトを作成
	TextureManager::RegisterLoadQue("./Engine/Resources/ErrorObject", "Error.png");
	PolygonMeshManager::RegisterLoadQue("./Engine/Resources/ErrorObject", "ErrorObject.obj");
	PolygonMeshManager::RegisterLoadQue("./Engine/Resources", "Grid.obj");
	BackgroundLoader::WaitEndExecute();

	light = std::make_unique<ConstantBuffer<DirectionalLightData>>(DirectionalLightData{ Color{ 1.0f,1.0f,1.0f,1.0f }, -Vec3::kBasisY, 1.0f });
	gridMesh = std::make_unique<GameObject>("Grid.obj");

	// オールコンプリート
	Log("Complete create D3D12Device\n");
}

void DirectXCore::begin_frame() {
	// BackBufferのステータスを反転
	DirectXSwapChain::ChangeBackBufferState();
	// RTVを設定
	DirectXSwapChain::SetRenderTarget();
	// ----------画面をクリア----------
	DirectXSwapChain::ClearScreen();

	// RootSignatureの設定
	pipelineState->set_root_signature();
	// PSOの設定
	pipelineState->set_graphics_pipeline_state();

	SRVDescriptorHeap::SetDescriptorHeaps();

	DirectXCommand::GetCommandList()->RSSetViewports(1, &viewPort);
	DirectXCommand::GetCommandList()->RSSetScissorRects(1, &scissorRect);
	DirectXCommand::GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	DirectXSwapChain::ClearDepthStencil();

#ifdef _DEBUG
	ImGuiManager::BeginFrame();
#endif // _DEBUG
	DirectXCommand::GetCommandList()->SetGraphicsRootConstantBufferView(3, light->get_resource()->GetGPUVirtualAddress());
}

void DirectXCore::end_frame() {
#ifdef _DEBUG
	// 一番先にImGUIの処理
	ImGuiManager::EndFrame();
#endif // _DEBUG
	// BackBufferのステータスを反転
	DirectXSwapChain::ChangeBackBufferState();
	// クローズしてエクスキュート
	DirectXCommand::GetInstance().close_and_kick();
	// スワップチェイン実行
	DirectXSwapChain::SwapScreen();
	// コマンド終了まで待つ
	DirectXCommand::GetInstance().wait_command();
	// コマンドリセット
	DirectXCommand::GetInstance().reset();
}

#ifdef _DEBUG
void DirectXCore::show_debug_tools() {
	ImGuiID debugDock = ImGui::GetID("DebugDock");

	ImGui::SetNextWindowDockID(debugDock, 0);
	Camera3D::DebugGUI();

	ImGui::SetNextWindowDockID(debugDock, 0);
	Camera2D::DebugGUI();

	ImGui::SetNextWindowSize(ImVec2{ 330,165 }, ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2{ 50, 235 }, ImGuiCond_Once);
	ImGui::SetNextWindowDockID(debugDock, 0);
	ImGui::Begin("Light", nullptr, ImGuiWindowFlags_NoSavedSettings);
	light->get_data()->color.debug_gui();
	Vector3 rotate = Vec3::kZero;
	ImGui::Text(std::format("X : {:.3}, Y : {:.3}, Z : {:.3}", light->get_data()->direction.x, light->get_data()->direction.y, light->get_data()->direction.z).c_str());
	if (ImGui::DragFloat3("DirectionRotate", &rotate.x, 0.02f)) {
		light->get_data()->direction = Transform3D::Homogeneous(light->get_data()->direction, Quaternion { rotate, rotate.length() }.to_matrix());
	}
	ImGui::Text("ResetDirection");
	if (ImGui::Button("X")) {
		light->get_data()->direction = Vec3::kBasisX;
	}
	ImGui::SameLine();
	if (ImGui::Button("Y")) {
		light->get_data()->direction = Vec3::kBasisY;
	}
	ImGui::SameLine();
	if (ImGui::Button("Z")) {
		light->get_data()->direction = Vec3::kBasisZ;
	}
	ImGui::SameLine();
	if (ImGui::Button("-X")) {
		light->get_data()->direction = -Vec3::kBasisX;
	}
	ImGui::SameLine();
	if (ImGui::Button("-Y")) {
		light->get_data()->direction = -Vec3::kBasisY;
	}
	ImGui::SameLine();
	if (ImGui::Button("-Z")) {
		light->get_data()->direction = -Vec3::kBasisZ;
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
	static std::unique_ptr<DirectXCore::Debug> instance{ new Debug };
	return *instance;
}
