#include "DirectXSwapChain.h"

#include <cassert>
#include <memory>

#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/DirectX/DirectXDevice/DirectXDevice.h"
#include "Engine/DirectX/DirectXResourceObject/OffscreenRender/OffscreenRender.h"
#include "Engine/WinApp.h"

DirectXSwapChain::DirectXSwapChain() noexcept {
	// 最初は描画していない状態
	backBufferIndex = 0;
	depthStencil = std::make_unique<DepthStencil>();
	depthStencil->initialize();
	clearColor = { 0.1f,0.25f, 0.5f, 1.0f };
}

void DirectXSwapChain::Initialize() {
	GetInstance().create_swapchain();
	GetInstance().create_offscreen();
}

void DirectXSwapChain::SetOffscreenRenderTarget() {
	// ----------オフスクリーンを描画先として設定----------
	DirectXCommand::GetCommandList()->OMSetRenderTargets(
		1,
		&GetInstance().offscreen->get_cpu_handle(),
		false,
		&GetInstance().depthStencil->get_cpu_handle()
	);
}

void DirectXSwapChain::SetOnscreenRenderTarget() {
	// ----------描画先のRTVを設定----------
	DirectXCommand::GetCommandList()->OMSetRenderTargets(
		1,
		&GetInstance().renderTarget[GetBackBufferIndex()].get_cpu_handle(),
		false,
		&GetInstance().depthStencil->get_cpu_handle()
	);
}

void DirectXSwapChain::RenderingOffscreen() {
	GetInstance().offscreen->draw();
}

void DirectXSwapChain::SwapScreen() {
	GetInstance().swap_screen();
}

void DirectXSwapChain::ChangeBackBufferState() {
	GetInstance().change_back_buffer_state();
}

void DirectXSwapChain::ChangeOffscreenState() {
	GetInstance().change_offscreen_state();
}

void DirectXSwapChain::ClearScreen() {
	// クリアする色
	DirectXCommand::GetCommandList()->ClearRenderTargetView(
		GetInstance().renderTarget[GetInstance().backBufferIndex].get_cpu_handle(),
		&GetInstance().clearColor.red, 0, nullptr
	);
	DirectXCommand::GetCommandList()->ClearRenderTargetView(
		GetInstance().offscreen->get_cpu_handle(),
		&GetInstance().clearColor.red, 0, nullptr
	);
}

void DirectXSwapChain::ClearDepthStencil() {
	DirectXCommand::GetCommandList()->ClearDepthStencilView(
		GetInstance().depthStencil->get_cpu_handle(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr
	);
}

void DirectXSwapChain::SetClearColor(const Color& color_) noexcept {
	GetInstance().clearColor = color_;
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
	fullscreenDesc.RefreshRate.Numerator = 15;
	fullscreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	fullscreenDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	fullscreenDesc.Windowed = true;

	// コマンドキュー、ウィンドウハンドル、設定を渡してスワップチェインを生成
	hr = DirectXDevice::GetFactory()->CreateSwapChainForHwnd(DirectXCommand::GetCommandQueue().Get(), WinApp::GetWndHandle(), &swapChainDesc, &fullscreenDesc, nullptr, reinterpret_cast<IDXGISwapChain1**>(swapChain.GetAddressOf()));
	// 失敗したら停止させる
	assert(SUCCEEDED(hr));
	// RTVにリソースを生成
	// ダブルバッファなのでリソースを2つ作る
	for (uint32_t renderIndex = 0; renderIndex < SWAPCHAIN_HEAP; ++renderIndex) {
		hr = swapChain->GetBuffer(renderIndex, IID_PPV_ARGS(renderTarget[renderIndex].get_resource().GetAddressOf()));
		assert(SUCCEEDED(hr));
		// view作成
		renderTarget[renderIndex].create_view();
	}
}

void DirectXSwapChain::create_offscreen() {
	offscreen = std::make_unique<OffscreenRender>();
	offscreen->initialize();
}

void DirectXSwapChain::change_back_buffer_state() {
	// ----------リソースバリアの設定----------
	renderTarget[backBufferIndex].change_buffer_state();
}

void DirectXSwapChain::change_offscreen_state() {
	offscreen->change_buffer_state();
}

void DirectXSwapChain::swap_screen() {
	swapChain->Present(0, 0);
	backBufferIndex = swapChain->GetCurrentBackBufferIndex();
}
