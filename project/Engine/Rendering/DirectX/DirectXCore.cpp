#include "Engine/Rendering/DirectX/DirectXCore.h"

#include <memory>

#include <dxgidebug.h>
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

#include "Engine/Debug/Output.h"
#include "Engine/Rendering/DirectX/DirectXDevice/DirectXDevice.h"
#include "Engine/Rendering/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/Rendering/DirectX/DirectXDescriptorHeap/RTVDescriptorHeap/RTVDescriptorHeap.h"
#include "Engine/Rendering/DirectX/DirectXDescriptorHeap/SRVDescriptorHeap/SRVDescriptorHeap.h"
#include "Engine/Rendering/DirectX/DirectXDescriptorHeap/DSVDescroptorHeap/DSVDescriptorHeap.h"
#include "Engine/Rendering/DirectX/DirectXResourceObject/DepthStencil/DepthStencil.h"
#include "Engine/Rendering/DirectX/DirectXSwapChain/DirectXSwapChain.h"
#include "Engine/Rendering/DirectX/ShaderCompiler/ShaderCompiler.h"

static HRESULT hr;

DirectXCore::DirectXCore() = default;

DirectXCore::~DirectXCore() = default;

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
	// DepthStencilの初期化
	DepthStencilValue::depthStencil = std::make_unique<DepthStencil>();
	DepthStencilValue::depthStencil->initialize(DXGI_FORMAT_D32_FLOAT);

	// オールコンプリート
	Console("Complete create DirectXObjects\n");
}

void DirectXCore::BeginFrame() {
	// srvの設定
	SRVDescriptorHeap::SetDescriptorHeaps();
}

void DirectXCore::EndFrame() {
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
	// DepthStencilInstanceを削除
	DepthStencilValue::depthStencil.reset();

	DirectXSwapChain::Finalize();
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
	static DirectXCore::Debug instance{};
	return instance;
}
