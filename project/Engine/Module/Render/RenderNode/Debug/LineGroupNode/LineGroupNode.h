#pragma once

#ifdef _DEBUG

#include "Engine/Module/Render/RenderNode/SingleRenderTargetNode.h"

class LineGroupNode final : public SingleRenderTargetNode {
public:
	LineGroupNode();
	~LineGroupNode() noexcept;

	LineGroupNode(const LineGroupNode&) = delete;
	LineGroupNode& operator=(const LineGroupNode&) = delete;
	LineGroupNode(LineGroupNode&&) = default;
	LineGroupNode& operator=(LineGroupNode&&) = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	//void use() override;

private:
	void create_pipeline_state();
};

#endif // _DEBUG
