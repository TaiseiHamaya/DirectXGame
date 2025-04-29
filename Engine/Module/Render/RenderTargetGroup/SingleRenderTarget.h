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
	void initialize() override;

	/// <summary>
	/// サイズ指定付き初期化
	/// </summary>
	/// <param name="width">幅</param>
	/// <param name="height">高さ</param>
	void initialize(u32 width, u32 height);

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
