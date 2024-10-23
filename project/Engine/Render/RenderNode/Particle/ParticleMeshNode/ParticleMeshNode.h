#pragma once

#include "Engine/Render/RenderNode/SingleRenderTargetNode.h"

class ParticleMeshNode final : public SingleRenderTargetNode {
public:
	ParticleMeshNode();
	~ParticleMeshNode() noexcept;

	ParticleMeshNode(const ParticleMeshNode&) = delete;
	ParticleMeshNode& operator=(const ParticleMeshNode&) = delete;
	ParticleMeshNode(ParticleMeshNode&&) = default;
	ParticleMeshNode& operator=(ParticleMeshNode&&) = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

private:
	void create_pipeline_state();
};
