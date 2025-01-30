#pragma once

#include <optional>

#include "Engine/Application/EngineSettings.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/DxResource.h"

class DepthStencil final : public DxResource {
public:
	DepthStencil() = default;
	~DepthStencil();

public:
	void initialize(DXGI_FORMAT format, std::uint32_t width = EngineSettings::CLIENT_WIDTH, std::uint32_t height = EngineSettings::CLIENT_HEIGHT);

	void change_resource_state();
	void release_index();

public:
	const D3D12_CPU_DESCRIPTOR_HANDLE& get_dsv_cpu_handle() const noexcept;
	const D3D12_DEPTH_STENCIL_DESC& get_desc() const noexcept;
	D3D12_GPU_DESCRIPTOR_HANDLE texture_gpu_handle() const;
	DXGI_FORMAT texture_format() const;

private:
	void create_depth_stencil_texture_resource(std::uint32_t width, std::uint32_t height, DXGI_FORMAT format);
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


#include <memory>
namespace DepthStencilValue {

inline std::shared_ptr<DepthStencil> depthStencil = nullptr;

}