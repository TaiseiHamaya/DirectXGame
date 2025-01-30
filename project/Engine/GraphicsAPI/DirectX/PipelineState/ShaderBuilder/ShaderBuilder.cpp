#include "ShaderBuilder.h"

#include <cassert>
#include <string>

#include <Library/Utility/Tools/ConvertString.h>

#include "Engine/GraphicsAPI/DirectX/DxSystemValues.h"
#include "Engine/GraphicsAPI/DirectX/ShaderCompiler/ShaderCompiler.h"

using namespace std::literals::string_literals;

void ShaderBuilder::initialize(const std::string& vertexShaderFilePath, const std::string& pixelShaderFilePath) {
	create_vertex_shader(vertexShaderFilePath);
	create_pixel_shader(pixelShaderFilePath);
}

D3D12_SHADER_BYTECODE ShaderBuilder::get_vs_bytecode() const noexcept {
	return { vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize() };
}

D3D12_SHADER_BYTECODE ShaderBuilder::get_ps_bytecode() const noexcept {
	return { pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize() };
}

void ShaderBuilder::create_vertex_shader(const std::string& filePath) {
	// VSをコンパル
	vertexShaderBlob = ShaderCompiler::GetInstance().compile_shader(ConvertString(filePath), L"vs_"s + DxSystemValues::SHADER_VERSION.data());
	assert(vertexShaderBlob);
}

void ShaderBuilder::create_pixel_shader(const std::string& filePath) {
	// PSをコンパイル
	pixelShaderBlob = ShaderCompiler::GetInstance().compile_shader(ConvertString(filePath), L"ps_"s + DxSystemValues::SHADER_VERSION.data());
	assert(pixelShaderBlob);
}
