#pragma once

#include "Engine/Render/RenderTargetGroup/BaseRenderTargetGroup.h"

#include <vector>

class OffscreenRender;

/// <summary>
/// 作成途中のため現在使用禁止
/// </summary>
class MultiRenderTarget final : public BaseRenderTargetGroup {
public:
	MultiRenderTarget();
	~MultiRenderTarget() noexcept;

	const std::vector<OffscreenRender>& offscreen_render_list();

private:
	std::vector<OffscreenRender> renderTargets;
};

