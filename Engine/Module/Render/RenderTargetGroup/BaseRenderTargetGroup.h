#pragma once

#include <memory>

#include <Library/Math/Color4.h>
#include <Library/Utility/Template/bitflag.h>
#include <Library/Utility/Template/Reference.h>

struct D3D12_VIEWPORT;
struct tagRECT;
class DepthStencilTexture;
enum class RenderNodeConfig : u8;

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
	virtual void begin_write(const eps::bitflag<RenderNodeConfig>& config, Reference<DepthStencilTexture> depthStencil);

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

	/// <summary>
	/// ViewPortの生成(基本はRTと同じ)
	/// </summary>
	/// <param name="width">幅</param>
	/// <param name="height">高さ</param>
	void create_view_port(u32 width, u32 height);

protected:
	Color4 clearColor{ CColor4::BLACK };
	std::unique_ptr<D3D12_VIEWPORT> viewPort;
	std::unique_ptr<tagRECT> scissorRect;
};

