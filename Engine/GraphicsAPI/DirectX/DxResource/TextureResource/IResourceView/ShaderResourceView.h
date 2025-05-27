#pragma once

#include "IResourceView.h"

#include <d3d12.h>

class ShaderResourceView : public IResourceView<D3D12_GPU_DESCRIPTOR_HANDLE> {
public:
	ShaderResourceView() = default;
	~ShaderResourceView() = default;

	__CLASS_NON_COPYABLE(ShaderResourceView)

public:
	void release() override;
	void create(Reference<ITextureResource> resource, DXGI_FORMAT format) override;
	void create_dds(Reference<ITextureResource> resource, DXGI_FORMAT format);
	D3D12_RESOURCE_STATES use_state() const  override { return D3D12_RESOURCE_STATE_GENERIC_READ; };

public:
	void use(u32 index) const;

private:

};
