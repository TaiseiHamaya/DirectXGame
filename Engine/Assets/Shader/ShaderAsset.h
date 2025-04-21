#pragma once

#include <d3d12.h>
#include <dxcapi.h>
#include <wrl/client.h>

#include <filesystem>

#include <Library/Utility/Tools/ConstructorMacro.h>

class ShaderAsset final {
public:
	ShaderAsset() = default;
	~ShaderAsset() = default;

	__CLASS_NON_COPYABLE(ShaderAsset)

public:
	bool load(const std::filesystem::path& filePath);

	D3D12_SHADER_BYTECODE blob_bytecode() const noexcept;
	DxcBuffer blob_buffer() const noexcept;

private:
	Microsoft::WRL::ComPtr<IDxcBlob> blob;
};
