#pragma once

#include <optional>

#include "Engine/DirectX/DirectXResourceObject/DirectXResourceObject.h"
#include "Engine/WinApp.h"

class DepthStencil final : public DirectXResourceObject {
public:
	DepthStencil() = default;
	~DepthStencil() = default;

public:
	void initialize(std::uint32_t width = WinApp::GetClientWidth(), std::uint32_t height = WinApp::GetClientHight());
	const D3D12_CPU_DESCRIPTOR_HANDLE& get_dsv_cpu_handle() const noexcept;
	const D3D12_DEPTH_STENCIL_DESC& get_desc() const noexcept;
	D3D12_GPU_DESCRIPTOR_HANDLE texture_gpu_handle() const;
	void change_resource_state();

private:
	void create_depth_stencil_texture_resource(std::uint32_t width, std::uint32_t height);
	void create_dsv();
	void create_srv();

private:
	bool isWriting;

	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	std::optional<std::uint32_t> dsvHeapIndex;
	D3D12_CPU_DESCRIPTOR_HANDLE dsvCPUHandle;
	std::optional<std::uint32_t> srvHeapIndex;
	D3D12_CPU_DESCRIPTOR_HANDLE srvCPUHandle{};
	D3D12_GPU_DESCRIPTOR_HANDLE srvGPUHandle{};
};
