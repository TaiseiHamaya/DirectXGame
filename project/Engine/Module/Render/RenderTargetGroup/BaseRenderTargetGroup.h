#pragma once

#include <memory>

#include <Library/Utility/Template/bitflag.h>

struct D3D12_VIEWPORT;
struct tagRECT;
class DepthStencil;
enum class RenderNodeConfig : std::uint8_t;

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
	/// 描画処理の開始
	/// </summary>
	virtual void begin(const eps::bitflag<RenderNodeConfig>& config_, const std::shared_ptr<DepthStencil>& depthStencil);

	/// <summary>
	/// 描画処理の終了
	/// </summary>
	virtual void end(const eps::bitflag<RenderNodeConfig>& config_);

protected:
	/// <summary>
	/// レンダーターゲットの設定
	/// </summary>
	virtual void set_render_target(const std::shared_ptr<DepthStencil>& depth) = 0;

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
	std::unique_ptr<D3D12_VIEWPORT> viewPort;
	std::unique_ptr<tagRECT> scissorRect;
};

