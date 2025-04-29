#pragma once

#include "IResourceView.h"

#include <d3d12.h>

class DepthStencilView : public IResourceView<D3D12_CPU_DESCRIPTOR_HANDLE> {
public:
	DepthStencilView() = default;
	~DepthStencilView() = default;

	__CLASS_NON_COPYABLE(DepthStencilView)

public:
	void release() override;
	void create(Reference<ITextureResource> resource, DXGI_FORMAT format_) override;
	D3D12_RESOURCE_STATES use_state() const override { return D3D12_RESOURCE_STATE_DEPTH_WRITE; }

	void clear(r32 depth = 1.0f, u8 stencil = 0) const;
	DXGI_FORMAT get_format() const { return format; }

private:
	DXGI_FORMAT format{};
};
