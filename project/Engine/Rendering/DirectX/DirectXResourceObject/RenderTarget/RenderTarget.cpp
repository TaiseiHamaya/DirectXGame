#include "RenderTarget.h"

#include "Engine/Rendering/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/Rendering/DirectX/DirectXDescriptorHeap/RTVDescriptorHeap/RTVDescriptorHeap.h"
#include "Engine/Rendering/DirectX/DirectXDevice/DirectXDevice.h"

RenderTarget::~RenderTarget() noexcept {
	release_index();
}

const D3D12_CPU_DESCRIPTOR_HANDLE& RenderTarget::get_cpu_handle() const noexcept {
	return rtvCPUHandle;
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

void RenderTarget::clear_resource() {
	auto&& commandList = DirectXCommand::GetCommandList();
	commandList->ClearRenderTargetView(
		rtvCPUHandle,
		&clearColor.red, 0, nullptr
	);
}

void RenderTarget::create_view(DXGI_FORMAT format) {
	// ----------rtvの設定----------
	//resource->GetDesc().Format == DXGI_FORMAT_R8G8B8A8_UNORM ? DXGI_FORMAT_R8G8B8A8_UNORM_SRGB : resource->GetDesc().Format;
	rtvDesc.Format = format; // 出力結果をSRGBに変換して書き込み
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D; // 2Dテクスチャとして書き込む
	// 空いているアドレスをDescriptorHeapに問い合わせ
	rtvHeapIndex = RTVDescriptorHeap::UseHeapIndex();
	rtvCPUHandle = RTVDescriptorHeap::GetCPUHandle(rtvHeapIndex.value());
	// 生成
	DirectXDevice::GetDevice()->CreateRenderTargetView(resource.Get(), &rtvDesc, rtvCPUHandle);
}

void RenderTarget::release_index() {
	if (rtvHeapIndex.has_value()) {
		RTVDescriptorHeap::ReleaseIndex(rtvHeapIndex.value());
		rtvHeapIndex = std::nullopt;
	}
}

UINT RenderTarget::get_width() const {
	return static_cast<UINT>(resource->GetDesc().Width);
}

UINT RenderTarget::get_height() const {
	return resource->GetDesc().Height;
}

const D3D12_RENDER_TARGET_VIEW_DESC& RenderTarget::get_rtv_desc() const {
	return rtvDesc;
}

void RenderTarget::set_clear_color(Color4 color_) {
	clearColor = color_;
}
