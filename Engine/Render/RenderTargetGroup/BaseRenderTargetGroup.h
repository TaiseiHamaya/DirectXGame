#pragma once

class DepthStencil;
struct D3D12_VIEWPORT;
struct tagRECT;
class Color;

#include <memory>

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
	/// 初期化
	/// </summary>
	virtual void initialize() = 0;

	/// <summary>
	/// 終了処理
	/// </summary>
	virtual void finalize() = 0;

	/// <summary>
	/// 描画処理の開始
	/// </summary>
	virtual void begin();

	/// <summary>
	/// 描画処理の終了
	/// </summary>
	virtual void end();

public:
	/// <summary>
	/// RTのclearColorの設定
	/// </summary>
	/// <param name="color_"></param>
	void set_clear_color(const Color& color_);

	/// <summary>
	/// DepthStencilの設定
	/// </summary>
	/// <param name="depthStencil_"></param>
	void set_depth_stencil(const std::shared_ptr<DepthStencil>& depthStencil_);

	/// <summary>
	/// DepthStencilを所持しているか判定
	/// </summary>
	/// <returns></returns>
	bool has_depth();

protected:
	/// <summary>
	/// レンダーターゲットの設定
	/// </summary>
	virtual void set_render_target() = 0;

	/// <summary>
	/// リソースバリアの状態を変更
	/// </summary>
	virtual void change_render_target_state() = 0;

	/// <summary>
	/// ViewPortの生成(基本はRTと同じ)
	/// </summary>
	/// <param name="width">幅</param>
	/// <param name="height">高さ</param>
	void create_view_port(std::uint32_t width, std::uint32_t height);

protected:
	std::shared_ptr<DepthStencil> depthStencil;
	std::unique_ptr<D3D12_VIEWPORT> viewPort;
	std::unique_ptr<tagRECT> scissorRect;
	std::unique_ptr<Color> clearColor;
};

