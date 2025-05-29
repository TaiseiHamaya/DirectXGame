#pragma once

#include "ITextureResource.h"

#include "./IResourceView/ShaderResourceView.h"

class TempTexture final : public ITextureResource {
public:
	TempTexture() = default;
	~TempTexture();

	__CLASS_NON_COPYABLE(TempTexture)

public:
	void initialize();

	Reference<const ShaderResourceView> get_as_srv() const { return srv; }

	void start_copy_dest();
	void start_read();

private:
	void create_resource();

private:
	ShaderResourceView srv;
};
