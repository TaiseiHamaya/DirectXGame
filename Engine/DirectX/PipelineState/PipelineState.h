#pragma once

#include <memory>

#include <d3d12.h>
#include <wrl/client.h>

class RootSignature;
class InputLayout;
class BlendState;
class RasterizerState;
class ShaderManager;

class PipelineState final {
public:
	PipelineState();
	~PipelineState();

private:
	PipelineState(const PipelineState&) = delete;
	PipelineState operator=(const PipelineState&) = delete;

public:
	void initialize();
	void set_root_signature();
	void set_graphics_pipeline_state();

private:
	void create_pipeline_state();

private:
	std::unique_ptr<RootSignature> rootSignature;
	std::unique_ptr<InputLayout> inputLayout;
	std::unique_ptr<BlendState> blendState;
	std::unique_ptr<RasterizerState> rasterizerState;
	std::unique_ptr<ShaderManager> shaderManger;

	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState;
};
