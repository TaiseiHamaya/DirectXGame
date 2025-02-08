#include "DxSwapChain.h"

#include <cassert>
#include <format>
#include <memory>

#include "Engine/Application/EngineSettings.h"
#include "Engine/Application/WinApp.h"
#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"
#include "Engine/GraphicsAPI/DirectX/DxDevice/DxDevice.h"
#include "Engine/GraphicsAPI/DirectX/DxSystemValues.h"
#include "Engine/GraphicsAPI/RenderingSystemValues.h"
#include "Engine/Module/Render/RenderNode/BaseRenderNode.h"
#include "Engine/Module/Render/RenderTargetGroup/SwapChainRenderTargetGroup.h"

void DxSwapChain::Initialize() {
	auto& instance = GetInstance();
	instance.create_swapchain();
	instance.create_render_target();
	SetClearColor(RenderingSystemValues::DEFAULT_CLEAR_COLOR);
}

void DxSwapChain::Finalize() {
	auto& instance = GetInstance();
	instance.renderTarget.reset();
	instance.swapChain.Reset();
}

void DxSwapChain::SwapScreen() {
	GetInstance().swap_screen();
}

const std::shared_ptr<SwapChainRenderTargetGroup>& DxSwapChain::GetRenderTarget() {
	return GetInstance().renderTarget;
}

void DxSwapChain::SetClearColor(const Color4& color_) noexcept {
	for (RenderTarget& renderTargetItr : GetInstance().renderTarget->get_render_targets()) {
		renderTargetItr.set_clear_color(color_);
	}
}

void DxSwapChain::EndRenderTarget() {
	auto& instance = GetInstance();
	instance.renderTarget->end(RenderNodeConfig::Default);
}

DxSwapChain& DxSwapChain::GetInstance() noexcept {
	static DxSwapChain instance{};
	return instance;
}

void DxSwapChain::create_swapchain() {
	HRESULT hr;
	// ----------スワップチェインを生成----------
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = EngineSettings::CLIENT_WIDTH; // 画面の幅
	swapChainDesc.Height = EngineSettings::CLIENT_HEIGHT; // 画面の高さ
	swapChainDesc.Format = DxSystemValues::SWAPCHAIN_FORMAT; // 色の形式
	swapChainDesc.SampleDesc.Count = 1; // マルチサンプルしない
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 画面のターゲットとして利用
	swapChainDesc.BufferCount = RenderingSystemValues::NUM_BUFFERING; // ダブルバッファ
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // モニタに映したら、中身を破棄
#ifdef _DEBUG
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING; // 限界までフレームレートを出せるようにする
#endif // _DEBUG
	//DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullscreenDesc{};
	//fullscreenDesc.RefreshRate.Denominator = 1;
	//fullscreenDesc.RefreshRate.Numerator = 60;
	//fullscreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	//fullscreenDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	//fullscreenDesc.Windowed = true;

	// コマンドキュー、ウィンドウハンドル、設定を渡してスワップチェインを生成
	//hr = DxDevice::GetFactory()->CreateSwapChainForHwnd(DxCommand::GetCommandQueue().Get(), WinApp::GetWndHandle(), &swapChainDesc, &fullscreenDesc, nullptr, reinterpret_cast<IDXGISwapChain1**>(swapChain.GetAddressOf()));
	hr = DxDevice::GetFactory()->CreateSwapChainForHwnd(DxCommand::GetCommandQueue().Get(), WinApp::GetWndHandle(), &swapChainDesc, nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(swapChain.GetAddressOf()));
	// 失敗したら停止させる
	assert(SUCCEEDED(hr));
}

void DxSwapChain::create_render_target() {
	HRESULT hr;
	// RTVにリソースを生成
	// ダブルバッファなのでリソースを2つ作る
	renderTarget = std::make_shared<SwapChainRenderTargetGroup>();
	for (uint32_t renderIndex = 0; renderIndex < RenderingSystemValues::NUM_BUFFERING; ++renderIndex) {
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		hr = swapChain->GetBuffer(renderIndex, IID_PPV_ARGS(resource.GetAddressOf()));
		assert(SUCCEEDED(hr));
		// view作成
		renderTarget->set_resource(resource, renderIndex);
		resource->SetName(std::format(L"SwapChain-RTV{}", renderIndex).c_str());
	}
	renderTarget->initialize();
}

#ifdef _DEBUG
#include "Engine/Runtime/WorldClock/WorldClock.h"
#endif // _DEBUG

void DxSwapChain::swap_screen() {
#ifdef _DEBUG
	if (WorldClock::IsUnlimitedFPS()) {
		swapChain->Present(0, DXGI_PRESENT_ALLOW_TEARING);
	}
	else {
		swapChain->Present(0, 0);
	}
#else
	swapChain->Present(1, 0);
#endif // _DEBUG
	RenderingSystemValues::ChangeBackbufferIndex(swapChain->GetCurrentBackBufferIndex());
}
