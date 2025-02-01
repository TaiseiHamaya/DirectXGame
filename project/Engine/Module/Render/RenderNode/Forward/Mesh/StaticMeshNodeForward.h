#pragma once

#include "Engine/Module/Render/RenderNode/SingleRenderTargetNode.h"

class StaticMeshNodeForward final : public SingleRenderTargetNode {
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

	void set_render_target(const std::shared_ptr<SingleRenderTarget>& renderTarget_ = nullptr) override;

private:
	void create_pipeline_state();
};
