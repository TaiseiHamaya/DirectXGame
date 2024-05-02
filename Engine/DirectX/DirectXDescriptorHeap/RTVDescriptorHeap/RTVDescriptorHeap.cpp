#include "RTVDescriptorHeap.h"

#include <memory>
#include <cassert>

#include "Engine/DirectX/DirectXDevice/DirectXDevice.h"
#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/DirectX/DirectXSwapChain/DirectXSwapChain.h"

// ダブルバッファなのでHeapも2
constexpr uint32_t HEAPSIZE = 2;

void RTVDescriptorHeap::Initialize() {
	GetInstance();
	GetInstance().create_descriptor_heap();
	GetInstance().initialize();
}

void RTVDescriptorHeap::SetSwapChain(IDXGISwapChain4* const swapChain) {
	GetInstance().set_swap_chain(swapChain);
}

const Descriptor& RTVDescriptorHeap::GetDescriptor(uint32_t index) {
	return GetInstance().get_descriptor(index);
}

void RTVDescriptorHeap::SetRenderTarget() {
	// ----------描画先のRTVを設定----------
	DirectXCommand::GetCommandList()->OMSetRenderTargets(
		1, 
		&GetInstance().descriptors[DirectXSwapChain::GetSwapChain()->GetCurrentBackBufferIndex()].heapHandleCPU,
		false, 
		nullptr
	);
}

void RTVDescriptorHeap::create_descriptor_heap() {
	descriptorHeap = DirectXDescriptorHeap::CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, HEAPSIZE, false);
}

void RTVDescriptorHeap::initialize() {
	descriptors.resize(HEAPSIZE);
	heapStartCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	// RTVはGPUHandle参照不可
	//heapStartGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	incrementSize = DirectXDevice::GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	releasedHeap.clear();
}

void RTVDescriptorHeap::set_swap_chain(IDXGISwapChain4* const swapChain) {
	HRESULT hr;
	// ダブルバッファなのでリソースを2つ作る
	hr = swapChain->GetBuffer(0, IID_PPV_ARGS(descriptors[0].resource.GetAddressOf()));
	assert(SUCCEEDED(hr));
	hr = swapChain->GetBuffer(1, IID_PPV_ARGS(descriptors[1].resource.GetAddressOf()));
	assert(SUCCEEDED(hr)); // 失敗したら停止させる
	// ----------rtvの設定----------
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 出力結果をSRGBに変換して書き込み
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D; // 2Dテクスチャとして書き込む
	// まず1つ目を作る。1つ目はアドレスの最初。作る場所はこちらで指定する。
	descriptors[0].heapHandleCPU = get_cpu_handle(0);
	// 1つ目を生成
	DirectXDevice::GetDevice()->CreateRenderTargetView(descriptors[0].resource.Get(), &rtvDesc, descriptors[0].heapHandleCPU);
	// 2つ目のディスクリプタハンドルを計算
	descriptors[1].heapHandleCPU = get_cpu_handle(1);
	// 2つ目を生成
	DirectXDevice::GetDevice()->CreateRenderTargetView(descriptors[1].resource.Get(), &rtvDesc, descriptors[1].heapHandleCPU);
}

RTVDescriptorHeap& RTVDescriptorHeap::GetInstance() {
	static std::unique_ptr<RTVDescriptorHeap> instance{ new RTVDescriptorHeap };
	return *instance;
}
