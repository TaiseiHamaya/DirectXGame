#pragma once

#include <d3d12.h>
#include <wrl/client.h>

#include <Library/Utility/Tools/ConstructorMacro.h>

class DxResource {
public:
	DxResource() noexcept = default;
	virtual ~DxResource() noexcept = default;

	__CLASS_NON_COPYABLE(DxResource)

public:
	Microsoft::WRL::ComPtr<ID3D12Resource>& get_resource() noexcept;
	const Microsoft::WRL::ComPtr<ID3D12Resource>& get_resource() const noexcept;

protected:
	Microsoft::WRL::ComPtr<ID3D12Resource> resource;

public:
	[[nodiscard]] static Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInByte);
};
