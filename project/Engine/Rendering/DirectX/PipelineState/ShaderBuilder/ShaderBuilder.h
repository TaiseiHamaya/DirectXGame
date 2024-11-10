#pragma once

#include <d3d12.h>
#include <dxcapi.h>
#include <wrl/client.h>
#include <string>

class ShaderBuilder {
public:
	ShaderBuilder() noexcept = default;
	~ShaderBuilder() noexcept = default;

private:
	ShaderBuilder(const ShaderBuilder&) = delete;
	ShaderBuilder operator=(const ShaderBuilder&) = delete;

public:
	void initialize(
		const std::string& vertexShaderFilePath,
		const std::string& pixelShaderFilePath
	) noexcept(false);

	D3D12_SHADER_BYTECODE get_vs_bytecode() const noexcept;
	D3D12_SHADER_BYTECODE get_ps_bytecode() const noexcept;

private:
	void create_vertex_shader(const std::string& filePath);
	void create_pixel_shader(const std::string& filePath);

private:
	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob;
	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob;
};
