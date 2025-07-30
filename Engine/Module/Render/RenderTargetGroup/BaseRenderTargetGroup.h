#pragma once

#include <Library/Math/Color4.h>
#include <Library/Utility/Template/Reference.h>

class DepthStencilTexture;

class BaseRenderTargetGroup {
public:
	BaseRenderTargetGroup();
	virtual ~BaseRenderTargetGroup() noexcept;

	BaseRenderTargetGroup(const BaseRenderTargetGroup&) = delete;
	BaseRenderTargetGroup& operator=(const BaseRenderTargetGroup&) = delete;
	BaseRenderTargetGroup(BaseRenderTargetGroup&&) = default;
	BaseRenderTargetGroup& operator=(BaseRenderTargetGroup&&) = default;

public:
	/// <summary>
	/// 描画処理の開始
	/// </summary>
	virtual void begin_write(bool isClear, Reference<DepthStencilTexture> depthStencil = nullptr);

	void set_clear_color(const Color4& color) { clearColor = color; }

protected:
	/// <summary>
	/// レンダーターゲットの設定
	/// </summary>
	virtual void start_render_target(Reference<DepthStencilTexture> depthStencil) = 0;

	/// <summary>
	/// レンダーターゲットのクリア
	/// </summary>
	virtual void clear_render_target() = 0;

protected:
	Color4 clearColor{ CColor4::BLACK };
};

