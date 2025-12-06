#pragma once

#include "./ITextureResource.h"

#include "./IResourceView/ShaderResourceView.h"

namespace szg {

class StaticTexture final : protected ITextureResource {
public:
	StaticTexture() = default;
	~StaticTexture();

	SZG_CLASS_MOVE_ONLY(StaticTexture)

public:
	void initialize(Microsoft::WRL::ComPtr<ID3D12Resource> resource_, bool isCubemap = false);

	Reference<const ShaderResourceView> get_srv() const { return srv; }

	void set_name(const std::wstring& name) override;

private:
	ShaderResourceView srv;
};

}; // szg
