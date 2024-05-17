#pragma once

#include "Engine/DirectX/DirectXResourceObject/DirectXResourceObject.h"

class DepthStencil final : public DirectXResourceObject {
public:
	DepthStencil() = default;
	~DepthStencil() = default;

public:
	void initialize();
	const D3D12_CPU_DESCRIPTOR_HANDLE& get_cpu_handle() const;
	const D3D12_DEPTH_STENCIL_DESC& get_depth_stencil_desc() const;

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> create_depth_stencil_texture_resource();

private:
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandleCPU;
};
