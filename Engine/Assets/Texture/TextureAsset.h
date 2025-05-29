#pragma once

#include "Engine/GraphicsAPI/DirectX/DxResource/TextureResource/StaticTexture.h"

class TextureAsset {
public:
	void initialize(Microsoft::WRL::ComPtr<ID3D12Resource> resource, bool isCubemap);

public:
	u32 index() const { return gpuIndex; }
	D3D12_GPU_DESCRIPTOR_HANDLE handle() const;

private:
	u32 gpuIndex{ 0 };
	StaticTexture texture;

#ifdef DEBUG_FEATURES_ENABLE
public:
	void set_name(const std::wstring& _name);
	const std::string& name() const { return name_; }

private:
	std::string name_;
#endif // DEBUG_FEATURES_ENABLE
};
