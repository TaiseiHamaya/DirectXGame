#include "RenderTarget.h"

#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/DirectX/DirectXDescriptorHeap/RTVDescriptorHeap/RTVDescriptorHeap.h"
#include "Engine/DirectX/DirectXDevice/DirectXDevice.h"

const D3D12_CPU_DESCRIPTOR_HANDLE& RenderTarget::get_cpu_handle() const noexcept {
	return descriptorHandleCPU;
}

void RenderTarget::change_resource_state() {
	DirectXCommand::SetBarrier(
		resource,
		isRendering ? D3D12_RESOURCE_STATE_RENDER_TARGET : D3D12_RESOURCE_STATE_PRESENT,
		isRendering ? D3D12_RESOURCE_STATE_PRESENT : D3D12_RESOURCE_STATE_RENDER_TARGET
	);
	// 描画の状態を反転
	isRendering = isRendering ^ 0b1;
}

void RenderTarget::create_view() {
	// ----------rtvの設定----------
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 出力結果をSRGBに変換して書き込み
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D; // 2Dテクスチャとして書き込む
	// 空いているアドレスをDescriptorHeapに問い合わせ
	rtvHeapIndex = RTVDescriptorHeap::UseHeapIndex();
	descriptorHandleCPU = RTVDescriptorHeap::GetCPUHandle(rtvHeapIndex.value());
	// 生成
	DirectXDevice::GetDevice()->CreateRenderTargetView(resource.Get(), &rtvDesc, descriptorHandleCPU);
}

void RenderTarget::release_index() const {
	if (rtvHeapIndex.has_value()) {
		RTVDescriptorHeap::ReleaseIndex(rtvHeapIndex.value());
	}
}

UINT RenderTarget::get_width() const {
	return static_cast<UINT>(resource->GetDesc().Width);
}

UINT RenderTarget::get_height() const {
	return resource->GetDesc().Height;
}
