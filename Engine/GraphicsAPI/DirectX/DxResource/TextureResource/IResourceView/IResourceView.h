#pragma once

#include <optional>

#include <d3d12.h>
#include <dxgiformat.h>

#include <Library/Utility/Template/Reference.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

class ITextureResource;

template<typename Handle>
class IResourceView {
public:
	IResourceView() = default;
	~IResourceView() = default;

	__CLASS_NON_COPYABLE(IResourceView)

public:
	virtual void release() = 0;
	virtual void create(Reference<ITextureResource> resource, DXGI_FORMAT format) = 0;
	const Handle& handle() const { return _handle; };
	std::optional<u32> get_index() const { return index; }

	virtual D3D12_RESOURCE_STATES use_state() const = 0;

protected:
	std::optional<u32> index{};
	Handle _handle{};
};
