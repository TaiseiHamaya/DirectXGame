#pragma once

#include <d3d12.h>

class RasterizerState final {
public:
	RasterizerState() noexcept = default;
	~RasterizerState() noexcept = default;

private:
	RasterizerState(const RasterizerState&) = delete;
	RasterizerState operator=(const RasterizerState&) = delete;

public:
	void initialize() noexcept;
	const D3D12_RASTERIZER_DESC& get_desc() const noexcept { return rasterizerDesc; }

private:
	void create_rasterizer_state() noexcept;

private:
	D3D12_RASTERIZER_DESC rasterizerDesc{};
};

