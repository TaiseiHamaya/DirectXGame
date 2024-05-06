#pragma once

#include <d3d12.h>
#include <wrl.h>

class Descriptor {
public:
	Descriptor() = default;
	~Descriptor() = default;

public:
	Descriptor(const Descriptor&) = delete;
	Descriptor& operator=(const Descriptor&) = delete;
	// ムーブは許可
	Descriptor(Descriptor&& rhs) noexcept;
	Descriptor& operator=(Descriptor&& rhs) noexcept;

public:
	Microsoft::WRL::ComPtr<ID3D12Resource> resource;
};