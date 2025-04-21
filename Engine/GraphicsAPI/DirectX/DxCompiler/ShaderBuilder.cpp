#include "ShaderBuilder.h"

#include "Engine/Assets/Shader/ShaderLibrary.h"
#include "Engine/Assets/BackgroundLoader/BackgroundLoader.h"

using namespace std::literals::string_literals;

void ShaderBuilder::initialize(const std::filesystem::path& vertexShaderFilePath, const std::filesystem::path& pixelShaderFilePath) {
	ShaderLibrary::RegisterLoadQue(vertexShaderFilePath);
	ShaderLibrary::RegisterLoadQue(pixelShaderFilePath);
	BackgroundLoader::WaitEndExecute();

	vsBlob = ShaderLibrary::GetShader(vertexShaderFilePath.filename().string());
	psBlob = ShaderLibrary::GetShader(pixelShaderFilePath.filename().string());
}

D3D12_SHADER_BYTECODE ShaderBuilder::get_vs_bytecode() const noexcept {
	return vsBlob->blob_bytecode();
}

D3D12_SHADER_BYTECODE ShaderBuilder::get_ps_bytecode() const noexcept {
	return psBlob->blob_bytecode();
}

DxcBuffer ShaderBuilder::vs_buffer() const noexcept {
	return vsBlob->blob_buffer();
}

DxcBuffer ShaderBuilder::ps_buffer() const noexcept {
	return psBlob->blob_buffer();
}
