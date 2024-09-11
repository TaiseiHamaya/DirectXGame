#include "DirectXSwapChain.h"

#include <cassert>
#include <memory>

#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/DirectX/DirectXDevice/DirectXDevice.h"
#include "Engine/DirectX/DirectXResourceObject/RenderTarget/RenderTarget.h"
#include "Engine/WinApp.h"
#include "Engine/Render/RenderTargetGroup/SwapChainRenderTargetGroup.h"

#include <format>

DirectXSwapChain::DirectXSwapChain() noexcept {
	// 最初は描画していない状態
	backBufferIndex = 0;
}

void DirectXSwapChain::Initialize() {
	GetInstance().create_swapchain();
	GetInstance().create_render_terget();
	GetInstance().depthStencil = std::make_unique<DepthStencil>();
	GetInstance().depthStencil->initialize();
}

void DirectXSwapChain::SwapScreen() {
	GetInstance().swap_screen();
}

const std::shared_ptr<SwapChainRenderTargetGroup>& DirectXSwapChain::GetRenderTarget() {
	return GetInstance().renderTarget;
}

const std::shared_ptr<DepthStencil>& DirectXSwapChain::GetDepthStencil() noexcept {
	return GetInstance().depthStencil;
}

void DirectXSwapChain::SetClearColor(const Color& color_) noexcept {
	GetInstance().renderTarget->set_clear_color(color_);
}

void DirectXSwapChain::ChangeBackBufferState() {
	GetInstance().renderTarget->end();
}

DirectXSwapChain& DirectXSwapChain::GetInstance() noexcept {
	static std::unique_ptr<DirectXSwapChain> instance{ new DirectXSwapChain };
	return *instance;
}

void DirectXSwapChain::create_swapchain() {
	HRESULT hr;
	// ----------スワップチェインを生成----------
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = WinApp::GetClientWidth(); // 画面の幅
	swapChainDesc.Height = WinApp::GetClientHight(); // 画面の高さ
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 色の形式
	swapChainDesc.SampleDesc.Count = 1; // マルチサンプルしない
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 画面のターゲットとして利用
	swapChainDesc.BufferCount = SWAPCHAIN_HEAP; // ダブルバッファ
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // モニタに映したら、中身を破棄
	DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullscreenDesc{};
	fullscreenDesc.RefreshRate.Denominator = 1;
	fullscreenDesc.RefreshRate.Numerator = 60;
	fullscreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	fullscreenDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	fullscreenDesc.Windowed = true;

	// コマンドキュー、ウィンドウハンドル、設定を渡してスワップチェインを生成
	hr = DirectXDevice::GetFactory()->CreateSwapChainForHwnd(DirectXCommand::GetCommandQueue().Get(), WinApp::GetWndHandle(), &swapChainDesc, &fullscreenDesc, nullptr, reinterpret_cast<IDXGISwapChain1**>(swapChain.GetAddressOf()));
	// 失敗したら停止させる
	assert(SUCCEEDED(hr));
}

void DirectXSwapChain::create_render_terget() {
	HRESULT hr;
	// RTVにリソースを生成
	// ダブルバッファなのでリソースを2つ作る
	renderTarget = std::make_shared<SwapChainRenderTargetGroup>();
	for (uint32_t renderIndex = 0; renderIndex < SWAPCHAIN_HEAP; ++renderIndex) {
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		hr = swapChain->GetBuffer(renderIndex, IID_PPV_ARGS(resource.GetAddressOf()));
		assert(SUCCEEDED(hr));
		// view作成
		renderTarget->set_resource(resource, renderIndex);
		resource->SetName(std::format(L"SwapChain{}", renderIndex).c_str());
	}
	renderTarget->initialize();
}

void DirectXSwapChain::swap_screen() {
	swapChain->Present(0, 0);
	backBufferIndex = swapChain->GetCurrentBackBufferIndex();
}
