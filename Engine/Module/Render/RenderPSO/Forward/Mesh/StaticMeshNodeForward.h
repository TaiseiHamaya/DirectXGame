#pragma once

#include "Engine/Module/Render/RenderPSO/BaseRenderNode.h"

class StaticMeshNodeForward final : public BaseRenderNode {
public:
	StaticMeshNodeForward();
	~StaticMeshNodeForward() noexcept;

	StaticMeshNodeForward(const StaticMeshNodeForward&) = delete;
	StaticMeshNodeForward& operator=(const StaticMeshNodeForward&) = delete;
	StaticMeshNodeForward(StaticMeshNodeForward&&) = default;
	StaticMeshNodeForward& operator=(StaticMeshNodeForward&&) = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	void preprocess() override {};

private:
	void create_pipeline_state();
};
