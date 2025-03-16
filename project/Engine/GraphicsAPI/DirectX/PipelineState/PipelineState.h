#pragma once

#include <d3d12.h>
#include <wrl/client.h>

#include <string>

class PipelineState final {
public:
	PipelineState() noexcept = default;
	~PipelineState() noexcept = default;

private:
	PipelineState(const PipelineState&) = delete;
	PipelineState& operator=(const PipelineState&) = delete;
	PipelineState(PipelineState&&) = default;
	PipelineState& operator=(PipelineState&&) = default;

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
