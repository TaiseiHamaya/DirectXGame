#pragma once

#include "Engine/Render/RenderTargetGroup/BaseRenderTargetGroup.h"

class OffscreenRender;

class SingleRenderTarget : public BaseRenderTargetGroup {
public:
	SingleRenderTarget();
	~SingleRenderTarget() noexcept;

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
	void initialize(std::uint32_t width, std::uint32_t hight);

public:
	/// <summary>
	/// レンダーターゲットの取得
	/// </summary>
	/// <returns></returns>
	const OffscreenRender& main_offscreen_render();

private:
	/// <summary>
	/// レンダーターゲットの設定
	/// </summary>
	void set_render_target() override;

	/// <summary>
	/// リソースバリアの状態を変更
	/// </summary>
	void change_render_target_state() override;

private:
	std::unique_ptr<OffscreenRender> renderTarget;
};
