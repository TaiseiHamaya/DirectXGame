#pragma once

#include "ITextureResource.h"

#include "./IResourceView/ShaderResourceView.h"

namespace szg {

class TempTexture final : public ITextureResource {
public:
	TempTexture() = default;
	~TempTexture();

	SZG_CLASS_MOVE_ONLY(TempTexture)

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

}; // szg
