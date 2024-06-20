#include "ShaderManager.h"

#include <cassert>

#include "Engine/Utility/ShaderCompiler/ShaderCompiler.h"
#include "Engine/Utility/Utility.h"

void ShaderManager::initialize(const std::string& vertexShaderFilePath, const std::string& pixelShaderFilePath) {
	create_vertex_shader(vertexShaderFilePath);
	create_pixel_shader(pixelShaderFilePath);
}

D3D12_SHADER_BYTECODE ShaderManager::get_vs_bytecode() const noexcept {
	return { vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize() };
}

D3D12_SHADER_BYTECODE ShaderManager::get_ps_bytecode() const noexcept {
	return { pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize() };
}

void ShaderManager::create_vertex_shader(const std::string& filePath) {
	// VSをコンパル
	vertexShaderBlob = ShaderCompiler::GetInstance().compile_shader(ConvertString(filePath), L"vs_6_0");
	assert(vertexShaderBlob);
}

void ShaderManager::create_pixel_shader(const std::string& filePath) {
	// PSをコンパイル
	pixelShaderBlob = ShaderCompiler::GetInstance().compile_shader(ConvertString(filePath), L"ps_6_0");
	assert(pixelShaderBlob);
}
