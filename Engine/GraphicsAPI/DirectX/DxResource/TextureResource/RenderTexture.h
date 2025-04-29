#pragma once

#include "./ITextureResource.h"

#include <Library/Math/Color4.h>

#include "./IResourceView/RenderTargetView.h"
#include "./IResourceView/ShaderResourceView.h"
#include "Engine/Application/EngineSettings.h"

class RenderTexture final : public ITextureResource {
public:
	RenderTexture() = default;
	~RenderTexture();

	__CLASS_NON_COPYABLE(RenderTexture)

public:
	void initialize(DXGI_FORMAT format, u32 width_ = EngineSettings::CLIENT_WIDTH, u32 height_ = EngineSettings::CLIENT_HEIGHT);
	void start_write();
	void start_read();

	Reference<const RenderTargetView> get_as_rtv() const { return rtv; }
	Reference<const ShaderResourceView> get_as_srv() const { return srv; }

private:
	void create_resource(DXGI_FORMAT format);

private:
	Color4 color;
	RenderTargetView rtv;
	ShaderResourceView srv;
};
