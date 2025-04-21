#pragma once

#include <d3d12.h>
#include <wrl/client.h>

#include <string>

class DxPipelineState final {
public:
	DxPipelineState() noexcept = default;
	~DxPipelineState() noexcept = default;

private:
	DxPipelineState(const DxPipelineState&) = delete;
	DxPipelineState& operator=(const DxPipelineState&) = delete;
	DxPipelineState(DxPipelineState&&) = default;
	DxPipelineState& operator=(DxPipelineState&&) = default;

public:
	void initialize(
		Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_,
		Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_
	);
	void set_root_signature();
	void set_graphics_pipeline_state();
	void set_name(const std::string& name);

private:
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState;
};
