#pragma once

#include <d3d12.h>
#include <wrl/client.h>
#include <vector>

#include "Engine/DirectX/PipelineState/ShaderBuilder/ShaderBuilder.h"

class InputLayoutBuillder {
public:
	void add_cbv(const char* semanticName, UINT semanticIndex, DXGI_FORMAT format);
	const std::vector<D3D12_INPUT_ELEMENT_DESC>& build();

private:
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputElementDescs;
};

class RootSignatureBuilder {
public:
	Microsoft::WRL::ComPtr<ID3D12RootSignature> build();

public:
	void add_cbv(D3D12_SHADER_VISIBILITY visibility, UINT shaderRagister);
	void add_texture(D3D12_SHADER_VISIBILITY visibility);
	void descriptor_range();
	void sampler(D3D12_FILTER filter, D3D12_COMPARISON_FUNC func, D3D12_SHADER_VISIBILITY visibility, UINT shaderRagister);

private:
	std::vector<D3D12_ROOT_PARAMETER> rootParameters;
	std::vector<D3D12_DESCRIPTOR_RANGE> descriptorRanges;
	std::vector<D3D12_STATIC_SAMPLER_DESC> staticSamplers;
};

class PSOBuilder {
public:
	Microsoft::WRL::ComPtr<ID3D12PipelineState> build();
	Microsoft::WRL::ComPtr<ID3D12RootSignature> get_rootsignature();

public:
	void rootsignature(const Microsoft::WRL::ComPtr<ID3D12RootSignature>& rootSignature_);
	void inputlayout(const std::vector<D3D12_INPUT_ELEMENT_DESC>& layout);
	void shaders(const ShaderBuilder& shaders);
	void blendstate();
	void blendstate(D3D12_BLEND_DESC blendDesc);
	void rasterizerstate(D3D12_FILL_MODE fillMode = D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE cullMode = D3D12_CULL_MODE_BACK);
	void depthstencilstate(const D3D12_DEPTH_STENCIL_DESC& depthStencilDesc);
	void primitivetopologytype(D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType_ = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE); // 使用するトポロジーのタイプ

private:
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
};

