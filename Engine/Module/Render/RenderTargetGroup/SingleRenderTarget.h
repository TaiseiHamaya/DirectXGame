#pragma once

#include "Engine/Module/Render/RenderTargetGroup/BaseRenderTargetGroup.h"

#include <Library/Utility/Template/Reference.h>

class RenderTexture;
class RenderTargetView;
class DepthStencilTexture;

class SingleRenderTarget : public BaseRenderTargetGroup {
public:
	SingleRenderTarget() = default;
	~SingleRenderTarget() noexcept = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="renderTexture"></param>
	void initialize(Reference<RenderTexture> renderTexture);


public:
	void set_texture(Reference<RenderTexture> texture_);

private:
	void start_render_target(Reference<DepthStencilTexture> depthStencil) override;

	/// <summary>
	/// レンダーターゲットのクリア
	/// </summary>
	void clear_render_target() override;

private:
	Reference<RenderTexture> texture;
	Reference<const RenderTargetView> view;
};
