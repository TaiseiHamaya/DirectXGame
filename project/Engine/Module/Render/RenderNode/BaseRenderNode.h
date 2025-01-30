#pragma once

#include <memory>

#include <Library/Utility/Template/bitflag.h>

#include <Engine/Module/Render/RenderTargetGroup/BaseRenderTargetGroup.h>

class DepthStencil;
class PipelineState;
enum D3D_PRIMITIVE_TOPOLOGY;

enum class RenderNodeConfig : std::uint8_t {
	Default = 0,
	NoClearRenderTarget = 1 << 0,
	NoChangeStateBegin = 1 << 1,
	NoChangeStateEnd = 1 << 2,
	NoClearDepth = 1 << 3,
	NoChangeDepthStateBegin = 1 << 4,
	NoChangeDepthStateEnd = 1 << 5,

	ContinueDrawBefore = NoChangeStateEnd,
	ContinueDrawAfter = NoClearRenderTarget | NoChangeStateBegin,
	ContinueUseDpehtBefore = NoChangeDepthStateEnd,
	ContinueUseDpehtAfter = NoClearDepth | NoChangeDepthStateBegin,
};

// BitFlag型を使用
__USE_BITFLAG(RenderNodeConfig)

class BaseRenderNode {
public:
	BaseRenderNode();
	virtual ~BaseRenderNode() noexcept;

	BaseRenderNode(const BaseRenderNode&) = delete;
	BaseRenderNode& operator=(const BaseRenderNode&) = delete;
	BaseRenderNode(BaseRenderNode&&) = default;
	BaseRenderNode& operator=(BaseRenderNode&&) = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void initialize() = 0;

	/// <summary>
	/// 使用決定時処理
	/// </summary>
	virtual void preprocess() = 0;

	/// <summary>
	/// 描画開始
	/// </summary>
	virtual void begin();

	/// <summary>
	/// 描画処理の終了
	/// </summary>
	virtual void end();

	/// <summary>
	/// RTGroupの取得
	/// </summary>
	/// <returns></returns>
	const std::shared_ptr<BaseRenderTargetGroup>& get_render_target_group() const;

	void set_config(const eps::bitflag<RenderNodeConfig>& config_);

protected:
	std::shared_ptr<BaseRenderTargetGroup> renderTarget;
	std::shared_ptr<DepthStencil> depthStencil;
	std::unique_ptr<PipelineState> pipelineState;
	D3D_PRIMITIVE_TOPOLOGY primitiveTopology{};

	eps::bitflag<RenderNodeConfig> config;
};
