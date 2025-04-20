#pragma once

#include <d3d12.h>
#include <dxcapi.h>
#include <wrl/client.h>

#include <filesystem>

#include "Engine/Assets/Shader/ShaderAsset.h"

class ShaderBuilder {
public:
	ShaderBuilder() noexcept = default;
	~ShaderBuilder() noexcept = default;

private:
	ShaderBuilder(const ShaderBuilder&) = delete;
	ShaderBuilder operator=(const ShaderBuilder&) = delete;

public:
	void initialize(
		const std::filesystem::path& vertexShaderFilePath,
		const std::filesystem::path& pixelShaderFilePath
	) noexcept(false);

	D3D12_SHADER_BYTECODE get_vs_bytecode() const noexcept;
	D3D12_SHADER_BYTECODE get_ps_bytecode() const noexcept;

	DxcBuffer vs_buffer() const noexcept;
	DxcBuffer ps_buffer() const noexcept;

private:
	std::shared_ptr<const ShaderAsset> vsBlob;
	std::shared_ptr<const ShaderAsset> psBlob;
};
