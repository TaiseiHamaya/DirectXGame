#pragma once

#include <d3d12.h>
#include <dxcapi.h>
#include <wrl/client.h>

#include <Library/Utility/Tools/ConstructorMacro.h>

class ShaderAsset final {
public:
	ShaderAsset() = default;
	~ShaderAsset() = default;

	__CLASS_NON_COPYABLE(ShaderAsset)

public:
	void initialize(Microsoft::WRL::ComPtr<IDxcBlob> blob_) noexcept;

	D3D12_SHADER_BYTECODE blob_bytecode() const noexcept;
	DxcBuffer blob_buffer() const noexcept;

private:
	Microsoft::WRL::ComPtr<IDxcBlob> blob;
};
