#pragma once

#include <wrl.h>
#include <dxcapi.h>
#include <d3d12.h>

class ShaderManager {
public:
	ShaderManager() = default;
	~ShaderManager() = default;

private:
	ShaderManager(const ShaderManager&) = delete;
	ShaderManager operator=(const ShaderManager&) = delete;

public:
	void initialize();

	D3D12_SHADER_BYTECODE get_vs_bytecode();
	D3D12_SHADER_BYTECODE get_ps_bytecode();

private:
	void create_vertex_shader();
	void create_pixel_shader();


private:
	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob;
	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob;
};
