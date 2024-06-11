#include "RasterizerState.h"

void RasterizerState::initialize() noexcept {
	create_rasterizer_state();
}

void RasterizerState::create_rasterizer_state() noexcept {
	// RasterizerStateの設定
	// 背面カリング
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	// 塗りつぶし
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
}
