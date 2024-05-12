#pragma once

#include "Engine/DirectX/DirectXResourceObject/DirectXResourceObject.h"

class RenderTarget final : public DirectXResourceObject {
public:
	RenderTarget() = default;
	~RenderTarget() = default;

	void initialize();
	const D3D12_CPU_DESCRIPTOR_HANDLE& get_cpu_handle();

private:
	D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandleCPU;
};
