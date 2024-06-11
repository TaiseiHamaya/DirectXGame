#pragma once

#include <d3d12.h>
#include <dxcapi.h>
#include <wrl/client.h>

class ShaderManager {
public:
	ShaderManager() noexcept = default;
	~ShaderManager() noexcept = default;

private:
	ShaderManager(const ShaderManager&) = delete;
	ShaderManager operator=(const ShaderManager&) = delete;

public:
	void initialize() noexcept(false);

	D3D12_SHADER_BYTECODE get_vs_bytecode() noexcept;
	D3D12_SHADER_BYTECODE get_ps_bytecode() noexcept;

private:
	void create_vertex_shader();
	void create_pixel_shader();


private:
	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob;
	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob;
};
