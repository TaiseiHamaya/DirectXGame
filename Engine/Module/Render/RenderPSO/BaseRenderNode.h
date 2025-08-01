#pragma once

#include <memory>

class DxPipelineState;
enum D3D_PRIMITIVE_TOPOLOGY;

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
	void stack_pso_command();

protected:
	std::unique_ptr<DxPipelineState> pipelineState;
	D3D_PRIMITIVE_TOPOLOGY primitiveTopology{};
};
