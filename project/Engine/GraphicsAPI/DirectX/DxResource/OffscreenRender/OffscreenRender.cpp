#include "OffscreenRender.h"

#include <cassert>
#include <format>

#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"
#include "Engine/GraphicsAPI/DirectX/DxDescriptorHeap/SRVDescriptorHeap/SRVDescriptorHeap.h"
#include "Engine/GraphicsAPI/DirectX/DxDevice/DxDevice.h"

OffscreenRender::OffscreenRender() noexcept = default;

OffscreenRender::~OffscreenRender() noexcept = default;

void OffscreenRender::initialize(UINT64 width, UINT height, DXGI_FORMAT format) {
	resource.Reset();
	// Resource作成
	create_resource(width, height, format);
	// view作成
	RenderTarget::create_view(format);
	// テクスチャ用Viewの作成
	create_texture_view();

	resource->SetName(std::format(L"Offscreen-RTV{}, SRV{}", rtvHeapIndex.value(), srvHeapIndex.value()).c_str());
}

void OffscreenRender::create_resource(UINT64 width, UINT height, DXGI_FORMAT format) {
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
	D3D12_RESOURCE_DESC desc{};
	desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D; // 2Dテクスチャを作成
	desc.Alignment = 0;
	desc.Width = width;
	desc.Height = height;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = format;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET; // レンダーターゲットとして使用する

	HRESULT hr;
	hr = DxDevice::GetDevice()->CreateCommittedResource(
		&uploadHeapProperties,
		D3D12_HEAP_FLAG_ALLOW_DISPLAY, // ALLOW_DISPLAYとすることでRenderTargetとして使用できる
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // 最初はRead状態から開始
		nullptr,
		IID_PPV_ARGS(resource.GetAddressOf())
	);
	assert(SUCCEEDED(hr));
}

void OffscreenRender::change_resource_state() {
	// GENERIC_READとRENDER_TARGETを入れ替える
	DxCommand::SetBarrier(
		resource,
		isRendering ? D3D12_RESOURCE_STATE_RENDER_TARGET : D3D12_RESOURCE_STATE_GENERIC_READ,
		isRendering ? D3D12_RESOURCE_STATE_GENERIC_READ : D3D12_RESOURCE_STATE_RENDER_TARGET
	);
	// 描画の状態を反転
	isRendering = isRendering ^ 0b1;
}

D3D12_GPU_DESCRIPTOR_HANDLE OffscreenRender::texture_gpu_handle() const {
	return srvGPUHandle;
}

void OffscreenRender::create_texture_view() {
	// テクスチャ用ヒープの取得
	srvHeapIndex = SRVDescriptorHeap::UseHeapIndex();
	srvCPUHandle = SRVDescriptorHeap::GetCPUHandle(srvHeapIndex.value());
	srvGPUHandle = SRVDescriptorHeap::GetGPUHandle(srvHeapIndex.value());
	// ここは通常のテクスチャと同じ
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = resource->GetDesc().Format == DXGI_FORMAT_R8G8B8A8_UNORM ? DXGI_FORMAT_R8G8B8A8_UNORM_SRGB : resource->GetDesc().Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1; // 1枚しかないのでmiplevelsも1
	// ここも変わらない
	DxDevice::GetDevice()->CreateShaderResourceView(resource.Get(), &srvDesc, srvCPUHandle);
}

void OffscreenRender::release_index() {
	if (srvHeapIndex.has_value()) {
		// SRVIndexを返す
		SRVDescriptorHeap::ReleaseHeapIndex(srvHeapIndex.value());
		srvHeapIndex = std::nullopt;
	}
	// RTVも同じようにする
	RenderTarget::release_index();
}

