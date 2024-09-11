#pragma once

class PipelineState;
class BaseRenderTargetGroup;
enum D3D_PRIMITIVE_TOPOLOGY;

#include <memory>

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
	/// 終了処理
	/// </summary>
	virtual void finalize();

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

protected:
	std::shared_ptr<BaseRenderTargetGroup> renderTarget;
	std::unique_ptr<PipelineState> pipelineState;
	D3D_PRIMITIVE_TOPOLOGY primitiveTopology{};
};
