#pragma once

#include "Engine/DirectX/DirectXResourceObject/DirectXResourceObject.h"

class DepthStencil final : public DirectXResourceObject {
public:
	DepthStencil() = default;
	~DepthStencil() = default;

public:
	void initialize();
	const D3D12_CPU_DESCRIPTOR_HANDLE& get_cpu_handle() const noexcept;
	const D3D12_DEPTH_STENCIL_DESC& get_desc() const noexcept;

private:
	void create_depth_stencil_texture_resource();

private:
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandleCPU;
};
