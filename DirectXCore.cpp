#include "Engine/Core/DirectXCore.h"

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

#include "Engine/Util.h"
#include "Engine/Core/DirectXDevice/DirectXDevice.h"

static HRESULT hr;

DirectXCore* DirectXCore::instance = nullptr;

void DirectXCore::Initialize(const HWND& hWnd) {
	assert(!instance);
	instance = new DirectXCore{};
	instance->initialize(hWnd);
}

void DirectXCore::BeginFrame() {
	instance->begin_frame();
}

void DirectXCore::EndFrame() {
	instance->end_frame();
}

void DirectXCore::Finalize() {
}

void DirectXCore::initialize(const HWND& hWnd) {
	Debug::Initialize();

	DirectXDevice::Initialize();

	Debug::InfoQueue();
}

void DirectXCore::begin_frame() {
}

void DirectXCore::end_frame() {
}

DirectXCore::Debug::~Debug() {
	IDXGIDebug1* debug;
	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug)))) {
		debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
		debug->Release();
	}
}

void DirectXCore::Debug::Initialize() {
	GetInstance();

	ID3D12Debug1* debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(true);
		debugController->Release();
	}
}

void DirectXCore::Debug::InfoQueue() {
	// 警告、エラーを取得するよう設定
	ID3D12InfoQueue* infoQueue = nullptr;
	if (SUCCEEDED(DirectXDevice::GetDevice()->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
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
		infoQueue->Release();
	}
}

DirectXCore::Debug* const DirectXCore::Debug::GetInstance() {
	static std::unique_ptr<DirectXCore::Debug> instance;
	return instance.get();
}
