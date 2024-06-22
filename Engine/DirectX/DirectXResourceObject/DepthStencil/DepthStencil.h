#pragma once

#include "Engine/DirectX/DirectXResourceObject/DirectXResourceObject.h"
#include "Engine/WinApp.h"

class DepthStencil final : public DirectXResourceObject {
public:
	DepthStencil() = default;
	~DepthStencil() = default;

public:
	void initialize(std::uint32_t width = WinApp::GetClientWidth(), std::uint32_t height = WinApp::GetClientHight());
	const D3D12_CPU_DESCRIPTOR_HANDLE& get_cpu_handle() const noexcept;
	const D3D12_DEPTH_STENCIL_DESC& get_desc() const noexcept;

private:
	void create_depth_stencil_texture_resource(std::uint32_t width, std::uint32_t height);

private:
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandleCPU;
};
