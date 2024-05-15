#include "RootSignature.h"

#include <cassert>
#include "Engine/Utility/Utility.h"
#include "Engine/DirectX/DirectXDevice/DirectXDevice.h"

void RootSignature::initialize() {
	create_root_signature();
}

Microsoft::WRL::ComPtr<ID3D12RootSignature>& RootSignature::get_root_signature() {
	return rootSignature;
}

const Microsoft::WRL::ComPtr<ID3D12RootSignature>& RootSignature::get_root_signature() const {
	return rootSignature;
}

void RootSignature::create_root_signature() {
	HRESULT hr;
	// ----------RootSignatureの生成----------
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignatureDesc{};
	descriptionRootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT; // シリアライズしてバイナリにする

	D3D12_ROOT_PARAMETER rootParameters[4] = {}; // Pixel, Vertex, Textureの3つ
	// Vertex
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // ConstantBufferView
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; // VertexShader
	rootParameters[0].Descriptor.ShaderRegister = 0; // レジスタ番号0
	
	// Pixel
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // ConstantBufferView
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShader
	rootParameters[1].Descriptor.ShaderRegister = 0; // レジスタ番号0

	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {}; // デスクリプタの範囲
	descriptorRange[0].BaseShaderRegister = 0; // 0から始まり、
	descriptorRange[0].NumDescriptors = 1; // 要素数は1
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // 自動計算する
	// Texture
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // DescriptorTableを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderの処理
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange; // デスクリプタの中身の設定
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); // Tableで使用する数

	// Light
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // ConstantBufferView
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShader
	rootParameters[3].Descriptor.ShaderRegister = 1; // レジスタ番号1

	// パラメータを記録
	descriptionRootSignatureDesc.pParameters = rootParameters; // 設定した配列のポインタ
	descriptionRootSignatureDesc.NumParameters = _countof(rootParameters); // 配列のサイズ

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {}; // サンプラーの設定
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // Bilinearフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // 0-1範囲外はリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX; // すべてのMipmapを使う
	staticSamplers[0].ShaderRegister = 0; // レジスタ番号0
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う

	// サンプラーを記録
	descriptionRootSignatureDesc.pStaticSamplers = staticSamplers; // サンプラーを設定
	descriptionRootSignatureDesc.NumStaticSamplers = _countof(staticSamplers); // 要素数

	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob;
	// バイナリに変換
	hr = D3D12SerializeRootSignature(&descriptionRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, signatureBlob.GetAddressOf(), errorBlob.GetAddressOf());
	if (FAILED(hr)) {
		Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer())); // 失敗したらログを出す
		assert(false);
	}
	// 変換したバイナリからRootSignatureを生成
	hr = DirectXDevice::GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(rootSignature.GetAddressOf()));
	assert(SUCCEEDED(hr));
}
