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
#include "externals/imgui/imgui_impl_dx12.h"
#include "externals/imgui/imgui_impl_win32.h"
#endif // _DEBUG

#include "Engine/WinApp.h"
#include "Engine/Util.h"
#include "Engine/DirectX/DirectXDevice/DirectXDevice.h"
#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/DirectX/DirectXDescriptorHeap/RTVDescriptorHeap/RTVDescriptorHeap.h"
#include "Engine/DirectX/DirectXSwapChain/DirectXSwapChain.h"

static HRESULT hr;

void DirectXCore::Initialize(const HWND& hWnd) {
	Debug::Initialize();
	GetInstance();
	GetInstance().initialize(hWnd);
}

void DirectXCore::BeginFrame() {
	GetInstance().begin_frame();
}

void DirectXCore::EndFrame() {
	GetInstance().end_frame();
}

void DirectXCore::Finalize() {
}

DirectXCore& DirectXCore::GetInstance() {
	static std::unique_ptr<DirectXCore> instance{ new DirectXCore };
	return *instance;
}

void DirectXCore::initialize(const HWND& hWnd) {

	DirectXDevice::Initialize();

	Debug::InfoQueue();

	DirectXCommand::Initialize();

	RTVDescriptorHeap::Initialize();

	DirectXSwapChain::Initialize(hWnd);



	viewPort.Width = static_cast<FLOAT>(WinApp::GetClientWidth());
	viewPort.Height = static_cast<FLOAT>(WinApp::GetClientHight());
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;

	scissorRect.left = 0;
	scissorRect.right = static_cast<LONG>(WinApp::GetClientWidth());
	scissorRect.top = 0;
	scissorRect.bottom = static_cast<LONG>(WinApp::GetClientHight());

	// オールコンプリート
	Log("Complete create D3D12Device\n");
}

void DirectXCore::begin_frame() {
	DirectXSwapChain::ChangeBackBufferState();
	RTVDescriptorHeap::SetRenderTarget();
	// クリアする色
	float clearColor[] = { 0.1f,0.25f, 0.5f, 1.0f }; // RGBA
	// ----------画面をクリア----------
	DirectXCommand::GetCommandList()->ClearRenderTargetView(RTVDescriptorHeap::GetDescriptor(DirectXSwapChain::GetSwapChain()->GetCurrentBackBufferIndex()).heapHandleCPU, clearColor, 0, nullptr);

	//DirectXCommand::GetCommandList()->RSSetViewports(1, &viewPort);
	//DirectXCommand::GetCommandList()->RSSetScissorRects(1, &scissorRect);
}

void DirectXCore::end_frame() {
	DirectXSwapChain::ChangeBackBufferState();
	DirectXCommand::GetInstance().close_and_execute();
	DirectXSwapChain::GetSwapChain()->Present(1, 0); // スワップチェイン
	DirectXCommand::GetInstance().wait_for_command();
	DirectXCommand::GetInstance().reset();
}

DirectXCore::Debug::~Debug() {
	Microsoft::WRL::ComPtr<IDXGIDebug1> debug;
	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(debug.GetAddressOf())))) {
		debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
	}
}

void DirectXCore::Debug::Initialize() {
	GetInstance();

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
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
		};
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;
		infoQueue->PushStorageFilter(&filter);
	}
}

DirectXCore::Debug* const DirectXCore::Debug::GetInstance() {
	static std::unique_ptr<DirectXCore::Debug> instance{ new Debug };
	return instance.get();
}
