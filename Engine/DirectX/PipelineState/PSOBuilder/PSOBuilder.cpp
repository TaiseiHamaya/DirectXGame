#include "PSOBuilder.h"

#include <cassert>

#include "Engine/Utility/Utility.h"
#include "Engine/DirectX/DirectXDevice/DirectXDevice.h"

void InputLayoutBuillder::add_cbv(const char* semanticName, UINT semanticIndex, DXGI_FORMAT format) {
	D3D12_INPUT_ELEMENT_DESC desc{};
	desc.SemanticName = semanticName;
	desc.SemanticIndex = semanticIndex;
	desc.Format = format;
	desc.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT; // 末尾に追加
	inputElementDescs.emplace_back(std::move(desc));
}

const std::vector<D3D12_INPUT_ELEMENT_DESC>& InputLayoutBuillder::build() {
	return inputElementDescs;
}

Microsoft::WRL::ComPtr<ID3D12RootSignature> RootSignatureBuilder::build() {
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignatureDesc{};
	descriptionRootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT; // シリアライズしてバイナリにする
	descriptionRootSignatureDesc.pParameters = rootParameters.data(); // 設定した配列のポインタ
	descriptionRootSignatureDesc.NumParameters = static_cast<UINT>(rootParameters.size()); // 配列のサイズ
	descriptionRootSignatureDesc.pStaticSamplers = staticSamplers.data(); // サンプラーを設定
	descriptionRootSignatureDesc.NumStaticSamplers = static_cast<UINT>(staticSamplers.size()); // 要素数

	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob;
	HRESULT hr;
	// バイナリに変換
	hr = D3D12SerializeRootSignature(&descriptionRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, signatureBlob.GetAddressOf(), errorBlob.GetAddressOf());
	if (FAILED(hr)) {
		Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer())); // 失敗したらログを出す
		assert(false);
	}
	// 変換したバイナリからRootSignatureを生成
	hr = DirectXDevice::GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(rootSignature.GetAddressOf()));
	assert(SUCCEEDED(hr));
	return rootSignature;
}

void RootSignatureBuilder::add_cbv(D3D12_SHADER_VISIBILITY visibility, UINT shaderRagister) {
	D3D12_ROOT_PARAMETER rootParameter{};
	rootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // ConstantBufferView
	rootParameter.ShaderVisibility = visibility; // VertexShader
	rootParameter.Descriptor.ShaderRegister = shaderRagister; // レジスタ番号0
	rootParameters.emplace_back(std::move(rootParameter));
}

void RootSignatureBuilder::add_texture(D3D12_SHADER_VISIBILITY visibility) {
	D3D12_ROOT_PARAMETER rootParameter{};
	rootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // DescriptorTableを使う
	rootParameter.ShaderVisibility = visibility;
	rootParameter.DescriptorTable.pDescriptorRanges = descriptorRanges.data(); // デスクリプタの中身の設定
	rootParameter.DescriptorTable.NumDescriptorRanges = static_cast<UINT>(descriptorRanges.size()); // Tableで使用する数
	rootParameters.emplace_back(std::move(rootParameter));
}

void RootSignatureBuilder::descriptor_range() {
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.BaseShaderRegister = 0; // 0から始まり、
	descriptorRange.NumDescriptors = 1; // 要素数は1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // 自動計算する
	descriptorRanges.emplace_back(std::move(descriptorRange));
}

void RootSignatureBuilder::sampler(D3D12_SHADER_VISIBILITY visibility, UINT shaderRagister, D3D12_FILTER filter, D3D12_TEXTURE_ADDRESS_MODE textureMore, D3D12_COMPARISON_FUNC func) {
	D3D12_STATIC_SAMPLER_DESC staticSampler{}; // サンプラーの設定
	staticSampler.Filter = filter; // フィルタ
	staticSampler.AddressU = textureMore; // 0-1範囲外はリピート
	staticSampler.AddressV = textureMore;
	staticSampler.AddressW = textureMore;
	staticSampler.ComparisonFunc = func;
	staticSampler.MaxLOD = D3D12_FLOAT32_MAX; // すべてのMipmapを使う
	staticSampler.ShaderRegister = shaderRagister;
	staticSampler.ShaderVisibility = visibility;
	staticSamplers.emplace_back(std::move(staticSampler));
}

Microsoft::WRL::ComPtr<ID3D12PipelineState> PSOBuilder::build() {
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState;
	// これまでの設定をまとめる
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	HRESULT hr;
	hr = DirectXDevice::GetDevice()
		->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(graphicsPipelineState.GetAddressOf())); // PSOの生成
	assert(SUCCEEDED(hr));

	return graphicsPipelineState;
}

Microsoft::WRL::ComPtr<ID3D12RootSignature> PSOBuilder::get_rootsignature() {
	return rootSignature;
}

void PSOBuilder::rootsignature(const Microsoft::WRL::ComPtr<ID3D12RootSignature>& rootSignature_) {
	rootSignature = rootSignature_;
	graphicsPipelineStateDesc.pRootSignature = rootSignature.Get();
}

void PSOBuilder::inputlayout(const std::vector<D3D12_INPUT_ELEMENT_DESC>& layout) {
	graphicsPipelineStateDesc.InputLayout = { layout.data(), static_cast<UINT>(layout.size()) };
}

void PSOBuilder::shaders(const ShaderBuilder& shaders) {
	graphicsPipelineStateDesc.VS = shaders.get_vs_bytecode();
	graphicsPipelineStateDesc.PS = shaders.get_ps_bytecode();
}

void PSOBuilder::blendstate() {
	graphicsPipelineStateDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
}

void PSOBuilder::blendstate(D3D12_BLEND_DESC blendDesc) {
	graphicsPipelineStateDesc.BlendState = blendDesc;
}

void PSOBuilder::rasterizerstate(D3D12_FILL_MODE fillMode, D3D12_CULL_MODE cullMode) {
	graphicsPipelineStateDesc.RasterizerState.FillMode = fillMode;
	graphicsPipelineStateDesc.RasterizerState.CullMode = cullMode;
}

void PSOBuilder::depthstencilstate(const D3D12_DEPTH_STENCIL_DESC& depthStencilDesc) {
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
}

void PSOBuilder::primitivetopologytype(D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType_) {
	graphicsPipelineStateDesc.PrimitiveTopologyType = topologyType_; // 使用するトポロジーのタイプ
}
