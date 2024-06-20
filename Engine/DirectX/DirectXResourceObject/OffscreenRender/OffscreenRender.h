#pragma once

#include <memory>

#include "Engine/DirectX/DirectXResourceObject/RenderTarget/RenderTarget.h"
#include "Engine/WinApp.h"

class VertexBuffer;

class OffscreenRender : public RenderTarget {
public:
	OffscreenRender() noexcept;
	~OffscreenRender() noexcept;

public:
	void initialize(UINT64 width = WinApp::GetClientWidth() , UINT height = WinApp::GetClientHight());
	void change_buffer_state() override;
	void create_textue_view();
	void set_texture(std::uint32_t rootParamaterIndex = 0) const;
	void draw(std::uint32_t rootParamaterIndex = 0) const;

private:
	void create_resource(UINT64 width, UINT height);

public:
	std::unique_ptr<VertexBuffer> vertex;
	std::uint32_t heapIndex;
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
};

