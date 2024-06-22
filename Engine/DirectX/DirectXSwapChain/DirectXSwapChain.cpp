#include "DirectXSwapChain.h"

#include <cassert>
#include <memory>

#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/DirectX/DirectXDevice/DirectXDevice.h"
#include "Engine/DirectX/DirectXResourceObject/RenderTarget/RenderTarget.h"
#include "Engine/WinApp.h"
#include "Engine/DirectX/PipelineState/PSOBuilder/PSOBuilder.h"
#include "Engine/DirectX/PipelineState/PipelineState.h"
#include "Engine/DirectX/DirectXSwapChain/SwapChainRenderNode/SwapChainRenderNode.h"

DirectXSwapChain::DirectXSwapChain() noexcept {
	// 最初は描画していない状態
	backBufferIndex = 0;
}

void DirectXSwapChain::Initialize() {
	GetInstance().create_swapchain();
	GetInstance().create_render_terget_view();
	GetInstance().create_render_node();
}

void DirectXSwapChain::SetRenderTarget() {
	// ----------描画先のRTVを設定----------
	GetInstance().renderNode->begin();
}

void DirectXSwapChain::SwapScreen() {
	GetInstance().swap_screen();
}

void DirectXSwapChain::ChangeBackBufferState() {
	GetInstance().renderTarget[GetBackBufferIndex()].change_resource_state();
}

void DirectXSwapChain::SetPSOFromBuilder(const std::unique_ptr<PSOBuilder>& psoBuilder) {
	std::unique_ptr<PipelineState> pso = std::make_unique<PipelineState>();
	pso->initialize(psoBuilder->get_rootsignature(), psoBuilder->build());
	GetInstance().renderNode->set_pso(std::move(pso));
}

const DepthStencil& DirectXSwapChain::GetDepthStencil() noexcept {
	return GetInstance().renderNode->get_depth_stencil();
}

void DirectXSwapChain::SetClearColor(const Color& color_) noexcept {
	GetInstance().renderNode->set_clear_color(color_);
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
}

void DirectXSwapChain::create_render_terget_view() {
	HRESULT hr;
	// RTVにリソースを生成
	// ダブルバッファなのでリソースを2つ作る
	for (uint32_t renderIndex = 0; renderIndex < SWAPCHAIN_HEAP; ++renderIndex) {
		hr = swapChain->GetBuffer(renderIndex, IID_PPV_ARGS(renderTarget[renderIndex].get_resource().GetAddressOf()));
		assert(SUCCEEDED(hr));
		// view作成
		renderTarget[renderIndex].create_view();
	}
}

void DirectXSwapChain::create_render_node() {
	renderNode = std::make_unique<SwapChainRenderNode>();
	renderNode->initialize();
	renderNode->set_render_target(renderTarget);
}

void DirectXSwapChain::swap_screen() {
	swapChain->Present(0, 0);
	backBufferIndex = swapChain->GetCurrentBackBufferIndex();
}
