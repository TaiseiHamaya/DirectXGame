#pragma once

#include "./ITextureResource.h"

#include "./IResourceView/RenderTargetView.h"

class ScreenTexture final : public ITextureResource {
public:
	ScreenTexture() = default;
	~ScreenTexture();

	__CLASS_NON_COPYABLE(ScreenTexture)

public:
	void initialize(Microsoft::WRL::ComPtr<ID3D12Resource> resource_);
	void start_write();
	void start_present();

#ifdef DEBUG_FEATURES_ENABLE
	void start_read();
#endif // DEBUG_FEATURES_ENABLE


	Reference<const RenderTargetView> get_as_rtv() const { return rtv; }

private:
	RenderTargetView rtv;
};
