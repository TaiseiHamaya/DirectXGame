#include "DirectXSwapChain.h"

#include <memory>
#include <cassert>

#include "Engine/WinApp.h"
#include "Engine/DirectX/DirectXDevice/DirectXDevice.h"
#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/DirectX/DirectXDescriptorHeap/RTVDescriptorHeap/RTVDescriptorHeap.h"

DirectXSwapChain::DirectXSwapChain() {
	// 最初は描画していない状態
	isRendering = false;
	backBufferIndex = 0;
	depthStencil = std::make_unique<DepthStencil>();
	depthStencil->initialize();
}

void DirectXSwapChain::Initialize() {
	GetInstance();
	GetInstance().create_swapchain();
}

void DirectXSwapChain::SetRenderTarget() {	// ----------描画先のRTVを設定----------
	DirectXCommand::GetCommandList()->OMSetRenderTargets(
		1,
		&GetInstance().renderTarget[GetBackBufferIndex()].get_cpu_handle(),
		false,
		&GetInstance().depthStencil->get_cpu_handle()
	);
}

void DirectXSwapChain::SwapScreen() {
	GetInstance().swap_screen();
}

void DirectXSwapChain::ChangeBackBufferState() {
	GetInstance().change_back_buffer_state();
}

void DirectXSwapChain::ClearScreen() {
	// クリアする色
	float clearColor[] = { 0.1f,0.25f, 0.5f, 1.0f }; // RGBA
	DirectXCommand::GetCommandList()->ClearRenderTargetView(
		GetInstance().renderTarget[GetInstance().backBufferIndex].get_cpu_handle(),
		clearColor, 0, nullptr
	);
}

void DirectXSwapChain::ClearDepthStencil() {
	DirectXCommand::GetCommandList()->ClearDepthStencilView(
		GetInstance().depthStencil->get_cpu_handle(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr
	);
}

DirectXSwapChain& DirectXSwapChain::GetInstance() {
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
	swapChainDesc.BufferCount = 2; // ダブルバッファ
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // モニタに映したら、中身を破棄
	// コマンドキュー、ウィンドウハンドル、設定を渡してスワップチェインを生成
	hr = DirectXDevice::GetFactory()->CreateSwapChainForHwnd(DirectXCommand::GetCommandQueue().Get(), WinApp::GetWndHandle(), &swapChainDesc, nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(swapChain.GetAddressOf()));
	// 失敗したら停止させる
	assert(SUCCEEDED(hr));
	// RTVにリソースを生成
	// ダブルバッファなのでリソースを2つ作る
	for (uint32_t renderIndex = 0; renderIndex < SWAPCHAIN_HEAP; ++renderIndex) {
		hr = swapChain->GetBuffer(renderIndex, IID_PPV_ARGS(renderTarget[renderIndex].get_resource().GetAddressOf()));
		assert(SUCCEEDED(hr));
		renderTarget[renderIndex].initialize();
	}
}

void DirectXSwapChain::change_back_buffer_state() {
	// ----------リソースバリアの設定----------
	DirectXCommand::SetBarrier(
		renderTarget[backBufferIndex].get_resource().Get(),
		isRendering ? D3D12_RESOURCE_STATE_RENDER_TARGET : D3D12_RESOURCE_STATE_PRESENT,
		isRendering ? D3D12_RESOURCE_STATE_PRESENT : D3D12_RESOURCE_STATE_RENDER_TARGET
	);
	// 描画の状態を反転
	isRendering = isRendering ^ 0b1;
}

void DirectXSwapChain::swap_screen() {
	swapChain->Present(1, 0);
	backBufferIndex = swapChain->GetCurrentBackBufferIndex();
}
