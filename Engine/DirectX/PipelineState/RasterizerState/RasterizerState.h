#pragma once
#include <d3d12.h>

class RasterizerState {
public:
	RasterizerState() = default;
	~RasterizerState() = default;

private:
	RasterizerState(const RasterizerState&) = delete;
	RasterizerState operator=(const RasterizerState&) = delete;

public:
	void initialize();
	const D3D12_RASTERIZER_DESC& get_desc() const { return rasterizerDesc; }

private:
	void create_rasterizer_state();

private:
	D3D12_RASTERIZER_DESC rasterizerDesc{};
};

