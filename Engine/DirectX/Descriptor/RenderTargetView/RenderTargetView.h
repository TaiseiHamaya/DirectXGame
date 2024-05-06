#pragma once

#include "Engine/DirectX/Descriptor/Descriptor.h"

class RenderTargetView : public Descriptor {
public:
	RenderTargetView() = default;
	~RenderTargetView() = default;

	void initialize();
	const Microsoft::WRL::ComPtr<ID3D12Resource>& get_resource();
	const D3D12_CPU_DESCRIPTOR_HANDLE& get_cpu_handle();

private:
	D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandleCPU;
};
