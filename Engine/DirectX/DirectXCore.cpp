#include "Engine/DirectX/DirectXCore.h"

#include <assert.h>
#include <memory>
#include <format>

#include <dxgidebug.h>
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

#ifdef _DEBUG
#include "externals/imgui/imgui.h"
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
	// ----------後で直す!!!----------
#ifdef _DEBUG
	ImGuiManager::Finalize();
#endif // _DEBUG
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

	DirectXSwapChain::ClearDepthStencil();

#ifdef _DEBUG
	ImGuiManager::BeginFrame();
#endif // _DEBUG
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
