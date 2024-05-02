#include "DirectXSwapChain.h"

#include <memory>
#include <cassert>

#include "Engine/WinApp.h"
#include "Engine/DirectX/DirectXDevice/DirectXDevice.h"
#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/DirectX/DirectXDescriptorHeap/RTVDescriptorHeap/RTVDescriptorHeap.h"

DirectXSwapChain::DirectXSwapChain() {
	is_rendering = false;
}

void DirectXSwapChain::Initialize(const HWND& hWnd) {
	GetInstance();
	GetInstance().create_swapchain(hWnd);
}

void DirectXSwapChain::ChangeBackBufferState() {
	GetInstance().change_back_buffer_state();
}

DirectXSwapChain& DirectXSwapChain::GetInstance() {
	static std::unique_ptr<DirectXSwapChain> instance{ new DirectXSwapChain };
	return *instance.get();
}

void DirectXSwapChain::create_swapchain(const HWND& hWnd) {
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
	hr = DirectXDevice::GetFactory()->CreateSwapChainForHwnd(DirectXCommand::GetCommandQueue().Get(), hWnd, &swapChainDesc, nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(swapChain.GetAddressOf()));
	// 失敗したら停止させる
	assert(SUCCEEDED(hr));

	RTVDescriptorHeap::SetSwapChain(swapChain.Get());
}

void DirectXSwapChain::change_back_buffer_state() {
	// 書き込むバックバッファのインデックスを取得
	UINT backBufferIndex = swapChain->GetCurrentBackBufferIndex();
	// ----------リソースバリアの設定----------
	DirectXCommand::SetBarrier(
		RTVDescriptorHeap::GetDescriptor(backBufferIndex).resource.Get(),
		is_rendering ? D3D12_RESOURCE_STATE_RENDER_TARGET : D3D12_RESOURCE_STATE_PRESENT,
		is_rendering ? D3D12_RESOURCE_STATE_PRESENT : D3D12_RESOURCE_STATE_RENDER_TARGET
	);
	is_rendering = is_rendering ^ 0b1;
}
