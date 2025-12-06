#pragma once

#include "IResourceView.h"

#include <d3d12.h>

#include <Library/Math/Color4.h>

namespace szg {

class RenderTargetView : public IResourceView<D3D12_CPU_DESCRIPTOR_HANDLE> {
public:
	RenderTargetView() = default;
	~RenderTargetView() = default;

	SZG_CLASS_MOVE_ONLY(RenderTargetView)

public:
	void release() override;
	void create(Reference<ITextureResource> resource, DXGI_FORMAT format) override;
	D3D12_RESOURCE_STATES use_state() const override { return D3D12_RESOURCE_STATE_RENDER_TARGET; };

	void clear(const Color4& color = CColor4::BLACK) const;

private:

};

}; // szg
