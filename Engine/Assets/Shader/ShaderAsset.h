#pragma once

#include <d3d12.h>
#include <dxcapi.h>
#include <wrl/client.h>

#include <Library/Utility/Tools/ConstructorMacro.h>

/// <summary>
/// シェーダーアセット
/// </summary>
namespace szg {

class ShaderAsset final {
public:
	ShaderAsset(Microsoft::WRL::ComPtr<IDxcBlob> blob_) noexcept;
	~ShaderAsset() = default;

	SZG_CLASS_MOVE_ONLY(ShaderAsset)

public:
	D3D12_SHADER_BYTECODE blob_bytecode() const noexcept;
	DxcBuffer blob_buffer() const noexcept;

private:
	Microsoft::WRL::ComPtr<IDxcBlob> blob;
};

}; // szg
