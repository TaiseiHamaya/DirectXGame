#pragma once

#include <d3d12.h>

class BlendState {
public:
	BlendState() = default;
	~BlendState() = default;

private:
	BlendState(const BlendState&) = delete;
	BlendState operator=(const BlendState&) = delete;

public:
	void initialize();
	const D3D12_BLEND_DESC& get_desc() const { return blendDesc; }

private:
	void create_blend_state();

private:
	D3D12_BLEND_DESC blendDesc{};
};
