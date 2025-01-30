#pragma once

#include <d3d12.h>
#include <list>
#include <vector>
#include <wrl/client.h>

#include "Engine/GraphicsAPI/DirectX/PipelineState/ShaderBuilder/ShaderBuilder.h"

class DepthStencil;

class InputLayoutBuilder {
public:
	void add_element(const char* semanticName, UINT semanticIndex, DXGI_FORMAT format, UINT slot = 0);
	const std::vector<D3D12_INPUT_ELEMENT_DESC>& build();

private:
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputElementDescs;
};

class RootSignatureBuilder {
public:
	Microsoft::WRL::ComPtr<ID3D12RootSignature> build();

public:
	void add_cbv(D3D12_SHADER_VISIBILITY visibility, UINT shaderRagister);
	void add_structured(D3D12_SHADER_VISIBILITY visibility, UINT baseShaderRegister = 0, UINT numDescriptors = 1);
	void add_texture(D3D12_SHADER_VISIBILITY visibility, UINT baseShaderRegister = 0, UINT numDescriptors = 1);
	void sampler(
		D3D12_SHADER_VISIBILITY visibility,
		UINT shaderRegister,
		D3D12_FILTER filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_TEXTURE_ADDRESS_MODE textureMore = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_COMPARISON_FUNC func = D3D12_COMPARISON_FUNC_NEVER
	);

private:
	std::vector<D3D12_ROOT_PARAMETER> rootParameters;
	std::list<D3D12_DESCRIPTOR_RANGE> descriptorRanges;
	std::vector<D3D12_STATIC_SAMPLER_DESC> staticSamplers;
};

enum class BlendMode {
	None,
	Normal,
	Add,
	Subtract,
	Multily,
	Screen,
	LightingPath,
};

class PSOBuilder {
public:
	Microsoft::WRL::ComPtr<ID3D12PipelineState> build();
	Microsoft::WRL::ComPtr<ID3D12RootSignature> get_rootsignature();

public:
	void rootsignature(const Microsoft::WRL::ComPtr<ID3D12RootSignature>& rootSignature_);
	void inputlayout(const std::vector<D3D12_INPUT_ELEMENT_DESC>& layout);
	void shaders(const ShaderBuilder& shaders);
	void blendstate(BlendMode blendMode = BlendMode::None, uint32_t renderTarget = 0);
	void blendstate_only_write();
	void blendstate(D3D12_RENDER_TARGET_BLEND_DESC blendDesc, uint32_t renderTarget = 0);
	void rasterizerstate(D3D12_FILL_MODE fillMode = D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE cullMode = D3D12_CULL_MODE_BACK);
	void depthstencilstate(const DepthStencil& depthStencil);
	void depth_state(DXGI_FORMAT format, D3D12_DEPTH_WRITE_MASK mask = D3D12_DEPTH_WRITE_MASK_ALL, D3D12_COMPARISON_FUNC func = D3D12_COMPARISON_FUNC_LESS);
	void stencil_state(UINT8 read, D3D12_DEPTH_WRITE_MASK write);
	void front_face(D3D12_COMPARISON_FUNC func, D3D12_STENCIL_OP depthFail, D3D12_STENCIL_OP stencilFail, D3D12_STENCIL_OP stencilPass);
	void back_face(D3D12_COMPARISON_FUNC func, D3D12_STENCIL_OP depthFail, D3D12_STENCIL_OP stencilFail, D3D12_STENCIL_OP stencilPass);
	void primitivetopologytype(D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType_ = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE); // 使用するトポロジーのタイプ
	void rendertarget(DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);

private:
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
};
