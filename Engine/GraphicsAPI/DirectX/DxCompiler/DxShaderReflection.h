#pragma once

#include <vector>

#include <d3d12.h>
#include <d3d12shader.h>
#include <wrl/client.h>

class ShaderBuilder;
class RootSignatureBuilder;

class DxShaderReflection {
public:
	struct SamplerDesc {
		D3D12_FILTER filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		D3D12_TEXTURE_ADDRESS_MODE textureMore = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		D3D12_COMPARISON_FUNC func = D3D12_COMPARISON_FUNC_NEVER;
	};

public:
	//void initialize(const ShaderBuilder& shader);
	Microsoft::WRL::ComPtr<ID3D12RootSignature> build_root_signature(std::vector<SamplerDesc> descs);

private:
	DXGI_FORMAT guess_format(const D3D12_SIGNATURE_PARAMETER_DESC& desc);
	void guess_bind(RootSignatureBuilder& rootSignatureBuilder, D3D12_SHADER_VISIBILITY visibility, const D3D12_SHADER_INPUT_BIND_DESC& bindDesc);

public:
	const std::vector<D3D12_INPUT_ELEMENT_DESC>& input_elements() const { return inputElementDescs; }

private:
	Microsoft::WRL::ComPtr<ID3D12ShaderReflection> vsReflector;
	Microsoft::WRL::ComPtr<ID3D12ShaderReflection> psReflector;
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputElementDescs;
};
