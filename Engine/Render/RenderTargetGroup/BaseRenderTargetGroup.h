#pragma once

class DepthStencil;
struct D3D12_VIEWPORT;
struct tagRECT;
class Color;

#include <memory>

#include <Engine/Utility/Bitflag/bitflag.h>

class BaseRenderTargetGroup {
public:
	enum class RTGConfing {
		Default = 0,
		NoClearDepth = 1 << 0,
		NoClearRenderTarget = 1 << 1,
		NoChangeStateBegin = 1 << 2,
		NoChangeStateEnd = 1 << 3,

		ContinueDrawBefore = NoChangeStateEnd,
		ContinueDrawAfter = NoChangeStateBegin | NoClearRenderTarget | NoClearDepth,
	};

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
	virtual void begin(const eps::bitflag<RTGConfing>& config_);

	/// <summary>
	/// 描画処理の終了
	/// </summary>
	virtual void end(const eps::bitflag<RTGConfing>& config_);

public:
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
	/// レンダーターゲットのクリア
	/// </summary>
	virtual void clear_render_target() = 0;

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
};

