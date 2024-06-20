#pragma once

#include "Engine/DirectX/DirectXResourceObject/DirectXResourceObject.h"

class RenderTarget : public DirectXResourceObject {
public:
	RenderTarget() noexcept = default;
	virtual ~RenderTarget() noexcept = default;

public:
	const D3D12_CPU_DESCRIPTOR_HANDLE& get_cpu_handle() noexcept;
	virtual void change_buffer_state();
	void create_view();

protected:
	bool isRendering;
	D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandleCPU;
};
