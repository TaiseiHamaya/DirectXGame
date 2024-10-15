#pragma once

#include <Engine/Render/RenderNode/SingleRenderTargetNode.h>

class CircleGaugeNode : public SingleRenderTargetNode {
public:
	CircleGaugeNode() = default;
	~CircleGaugeNode() noexcept = default;

	CircleGaugeNode(const CircleGaugeNode&) = delete;
	CircleGaugeNode& operator=(const CircleGaugeNode&) = delete;
	CircleGaugeNode(CircleGaugeNode&&) = default;
	CircleGaugeNode& operator=(CircleGaugeNode&&) = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

private:
	void create_pipeline_state();
};
