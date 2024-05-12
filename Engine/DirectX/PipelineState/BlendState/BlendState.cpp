#include "BlendState.h"

void BlendState::initialize() {
	create_blend_state();
}

void BlendState::create_blend_state() {
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
}
