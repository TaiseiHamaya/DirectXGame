#pragma once

#include <cstdint>
#include <d3d12.h>
#include <wrl/client.h>

class DirectXResourceObject {
public:
	DirectXResourceObject() noexcept = default;
	virtual ~DirectXResourceObject() noexcept = default;

public:
	// コピーは禁止
	DirectXResourceObject(const DirectXResourceObject&) = delete;
	DirectXResourceObject& operator=(const DirectXResourceObject&) = delete;
	// ムーブは許可
	DirectXResourceObject(DirectXResourceObject&& rhs) noexcept;
	DirectXResourceObject& operator=(DirectXResourceObject&& rhs) noexcept;

public:
	Microsoft::WRL::ComPtr<ID3D12Resource>& get_resource() noexcept;
	const Microsoft::WRL::ComPtr<ID3D12Resource>& get_resource() const noexcept;

protected:
	Microsoft::WRL::ComPtr<ID3D12Resource> resource;

protected:
	[[nodiscard]] static Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(std::uint64_t sizeInByte);
};