#include "OffscreenRender.h"

#include <cassert>

#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/DirectX/DirectXDescriptorHeap/SRVDescriptorHeap/SRVDescriptorHeap.h"
#include "Engine/DirectX/DirectXDevice/DirectXDevice.h"
#include "Engine/DirectX/DirectXResourceObject/VertexBuffer/VertexBuffer.h"

OffscreenRender::OffscreenRender() noexcept = default;

OffscreenRender::~OffscreenRender() noexcept = default;

void OffscreenRender::initialize(UINT64 width, UINT height) {
	// Resource作成
	create_resource(width, height);
	// view作成
	RenderTarget::create_view();
	// テクスチャ用ヒープの取得
	heapIndex = SRVDescriptorHeap::GetNextHandleIndex();
	cpuHandle = SRVDescriptorHeap::GetCPUHandle(heapIndex);
	gpuHandle = SRVDescriptorHeap::GetGPUHandle(heapIndex);
	// テクスチャ用Viewの作成
	create_textue_view();
	// テクスチャ用頂点情報作成
	std::vector<VertexData> vertexData(6);
	vertexData[0].vertex = VertexData::Vector4{ {-1, 1, 0}, 1 };
	vertexData[0].texcoord = CVector2::ZERO;
	vertexData[1].vertex = VertexData::Vector4{ { 1, -1, 0}, 1 };
	vertexData[1].texcoord = CVector2::BASIS;
	vertexData[2].vertex = VertexData::Vector4{ {-1, -1, 0}, 1 };
	vertexData[2].texcoord = CVector2::BASIS_Y;
	vertexData[3] = vertexData[0];
	vertexData[4].vertex = VertexData::Vector4{ { 1, 1, 0}, 1 };
	vertexData[4].texcoord = CVector2::BASIS_X;
	vertexData[5] = vertexData[1];
	vertex = std::make_unique<VertexBuffer>(vertexData);
}

void OffscreenRender::create_resource(UINT64 width, UINT height) {
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
	D3D12_RESOURCE_DESC desc{};
	desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D; // 2Dテクスチャを作成
	desc.Alignment = 0;
	desc.Width = width;
	desc.Height = height;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET; // レンダーターゲットとして使用する

	HRESULT hr;
	hr = DirectXDevice::GetDevice()->CreateCommittedResource(
		&uploadHeapProperties,
		D3D12_HEAP_FLAG_ALLOW_DISPLAY, // ALLOW_DISPLAYとすることでRenderTargetとして使用できる
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(resource.GetAddressOf())
	);
	assert(SUCCEEDED(hr));
	resource->SetName(L"Offscreen");
}

void OffscreenRender::change_buffer_state() {
	// GENERIC_READとRENDER_TARGETを入れ替える
	DirectXCommand::SetBarrier(
		resource,
		isRendering ? D3D12_RESOURCE_STATE_RENDER_TARGET : D3D12_RESOURCE_STATE_GENERIC_READ,
		isRendering ? D3D12_RESOURCE_STATE_GENERIC_READ : D3D12_RESOURCE_STATE_RENDER_TARGET
	);
	// 描画の状態を反転
	isRendering = isRendering ^ 0b1;
}

void OffscreenRender::create_textue_view() {
	// ここは通常のテクスチャと同じ
	// 1枚しかないのでmiplevelsも1
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	DirectXDevice::GetDevice()->CreateShaderResourceView(resource.Get(), &srvDesc, cpuHandle);
}

void OffscreenRender::set_texture(std::uint32_t rootParamaterIndex) const {
	DirectXCommand::GetCommandList()->SetGraphicsRootDescriptorTable(rootParamaterIndex, gpuHandle);
}

void OffscreenRender::draw(std::uint32_t rootParamaterIndex) const {
	auto&& command = DirectXCommand::GetCommandList();
	command->IASetVertexBuffers(0, 1, vertex->get_p_vbv());
	set_texture(rootParamaterIndex);
	command->DrawInstanced(6, 1, 0, 0);
}
