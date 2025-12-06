#pragma once

#include "Engine/GraphicsAPI/DirectX/DxResource/TextureResource/StaticTexture.h"

namespace szg {

class TextureAsset {
public:
	TextureAsset(Microsoft::WRL::ComPtr<ID3D12Resource> resource, bool isCubemap);
	~TextureAsset() = default;

	SZG_CLASS_MOVE_ONLY(TextureAsset)

public:
	u32 index() const;
	D3D12_GPU_DESCRIPTOR_HANDLE handle() const;
	Reference<const ShaderResourceView> get_srv() const;

private:
	u32 gpuIndex{ 0 };
	StaticTexture texture;

#ifdef DEBUG_FEATURES_ENABLE
public:
	void set_name(const std::wstring& _name);
	const std::string& name() const;

private:
	std::string name_;
#endif // DEBUG_FEATURES_ENABLE
};

}; // szg
