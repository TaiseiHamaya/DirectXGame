#pragma once

#include "Engine/Module/Render/RenderNode/SingleRenderTargetNode.h"

class LineNode final : public SingleRenderTargetNode {
public:
	LineNode();
	~LineNode() noexcept;

	LineNode(const LineNode&) = delete;
	LineNode& operator=(const LineNode&) = delete;
	LineNode(LineNode&&) = default;
	LineNode& operator=(LineNode&&) = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	void preprocess() override {};

private:
	void create_pipeline_state();
};
