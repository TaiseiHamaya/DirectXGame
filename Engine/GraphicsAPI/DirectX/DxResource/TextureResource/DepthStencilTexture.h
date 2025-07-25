#pragma once

#include "./ITextureResource.h"

#include "./IResourceView/DepthStencilView.h"
#include "./IResourceView/ShaderResourceView.h"
#include "Engine/Application/ProjectSettings/ProjectSettings.h"

class DepthStencilTexture final : protected ITextureResource {
public:
	DepthStencilTexture() = default;
	~DepthStencilTexture();

	__CLASS_NON_COPYABLE(DepthStencilTexture)

public:
	void initialize(DXGI_FORMAT format, u32 width_ = ProjectSettings::ClientWidth(), u32 height_ = ProjectSettings::ClientHeight());
	void start_write();
	void start_read();

	Reference<const ShaderResourceView> get_as_srv() const { return srv; }
	Reference<const DepthStencilView> get_as_dsv() const { return dsv; }

private:
	void create_resource(DXGI_FORMAT format);

private:
	DepthStencilView dsv;
	ShaderResourceView srv;
};
