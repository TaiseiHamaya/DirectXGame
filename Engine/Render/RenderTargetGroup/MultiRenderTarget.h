#pragma once

#include "Engine/Render/RenderTargetGroup/BaseRenderTargetGroup.h"

#include <vector>

class OffscreenRender;
struct D3D12_CPU_DESCRIPTOR_HANDLE;

//template<uint32_t NumRenderTarget = 2>
class MultiRenderTarget final : public BaseRenderTargetGroup {
	//static_assert(NumRenderTarget >= 2, "NumRenderTargetは2以上である必要があります");
public:
	MultiRenderTarget();
	~MultiRenderTarget() noexcept;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	/// <summary>
	/// 終了処理
	/// </summary>
	void finalize() override;

	/// <summary>
	/// サイズ指定付き初期化
	/// </summary>
	/// <param name="width">幅</param>
	/// <param name="hight">高さ</param>
	/// <param name="size">レンダーターゲット数</param>
	void initialize(std::uint32_t width, std::uint32_t hight, std::uint32_t size);

	/// <summary>
	/// サイズ指定付き初期化
	/// </summary>
	/// <param name="size">レンダーターゲット数</param>
	void initialize(std::uint32_t size);

	std::vector<OffscreenRender>& offscreen_render_list();
	const std::vector<OffscreenRender>& offscreen_render_list() const;

private:
	/// <summary>
	/// レンダーターゲットの設定
	/// </summary>
	void set_render_target(const std::shared_ptr<DepthStencil>& depthStencil) override;

	/// <summary>
	/// レンダーターゲットのクリア
	/// </summary>
	void clear_render_target() override;

	/// <summary>
	/// リソースバリアの状態を変更
	/// </summary>
	void change_render_target_state() override;

private:
	std::vector<OffscreenRender> renderTargets;
	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> renderTargetsHandles;
};
