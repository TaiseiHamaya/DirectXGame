#include "ShaderManager.h"

#include <cassert>
#include "Engine/Utility/ShaderCompiler.h"

void ShaderManager::initialize() {
	create_vertex_shader();
	create_pixel_shader();
}

D3D12_SHADER_BYTECODE ShaderManager::get_vs_bytecode() {
	return { vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize() };
}

D3D12_SHADER_BYTECODE ShaderManager::get_ps_bytecode() {
	return { pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize() };
}

void ShaderManager::create_vertex_shader() {
	// VSをコンパル
	vertexShaderBlob = ShaderCompiler::GetInstance().compile_shader(L"Engine/HLSL/Object3d.VS.hlsl", L"vs_6_0");
	assert(vertexShaderBlob);

}

void ShaderManager::create_pixel_shader() {
	// PSをコンパイル
	pixelShaderBlob = ShaderCompiler::GetInstance().compile_shader(L"Engine/HLSL/Object3d.PS.hlsl", L"ps_6_0");
	assert(pixelShaderBlob);
}
