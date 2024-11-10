#pragma once

#include <Engine/Module/Render/RenderNode/SingleRenderTargetNode.h>

class BeamNode final : public SingleRenderTargetNode {
public:
	BeamNode() = default;
	~BeamNode() noexcept = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

private:
	void create_pipeline_state();
};

