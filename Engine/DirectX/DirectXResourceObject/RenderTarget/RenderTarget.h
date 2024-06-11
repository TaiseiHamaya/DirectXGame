#pragma once

#include "Engine/DirectX/DirectXResourceObject/DirectXResourceObject.h"

class RenderTarget final : public DirectXResourceObject {
public:
	RenderTarget() noexcept = default;
	~RenderTarget() noexcept = default;

	void initialize();
	const D3D12_CPU_DESCRIPTOR_HANDLE& get_cpu_handle() noexcept;

private:
	D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandleCPU;
};
