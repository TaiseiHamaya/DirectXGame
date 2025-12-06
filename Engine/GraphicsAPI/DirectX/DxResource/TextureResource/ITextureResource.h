#pragma once

#include "../DxResource.h"

#include <string>

#include <Library/Utility/Tools/ConstructorMacro.h>

namespace szg {

class ITextureResource : public DxResource {
public:
	ITextureResource() = default;
	virtual ~ITextureResource() = default;

	SZG_CLASS_MOVE_ONLY(ITextureResource)

protected:
	void change_resource_state(D3D12_RESOURCE_STATES next);

public:
	u32 get_width() const { return width; }
	u32 get_height() const { return height; }

	virtual void set_name(const std::wstring& name);

protected:
	u32 width{ 0 };
	u32 height{ 0 };
	D3D12_RESOURCE_STATES state{};
};

}; // szg
