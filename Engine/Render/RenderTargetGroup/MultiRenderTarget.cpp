#include "MultiRenderTarget.h"

#include "Engine/DirectX/DirectXResourceObject/OffscreenRender/OffscreenRender.h"

MultiRenderTarget::MultiRenderTarget() = default;

MultiRenderTarget::~MultiRenderTarget() noexcept = default;

const std::vector<OffscreenRender>& MultiRenderTarget::offscreen_render_list() {
	return renderTargets;
}
