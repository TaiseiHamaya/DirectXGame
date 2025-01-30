#pragma once

#include <cstdint>
#include <d3d12.h>
#include <wrl/client.h>

class DxResource {
public:
	DxResource() noexcept = default;
	virtual ~DxResource() noexcept = default;

public:
	// コピーは禁止
	DxResource(const DxResource&) = delete;
	DxResource& operator=(const DxResource&) = delete;
	// ムーブは許可
	DxResource(DxResource&& rhs) noexcept;
	DxResource& operator=(DxResource&& rhs) noexcept;

public:
	Microsoft::WRL::ComPtr<ID3D12Resource>& get_resource() noexcept;
	const Microsoft::WRL::ComPtr<ID3D12Resource>& get_resource() const noexcept;

protected:
	Microsoft::WRL::ComPtr<ID3D12Resource> resource;

protected:
	[[nodiscard]] static Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(std::uint64_t sizeInByte);
};
