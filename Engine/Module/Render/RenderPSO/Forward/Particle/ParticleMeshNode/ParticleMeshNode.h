#pragma once

#include "Engine/Module/Render/RenderPSO/BaseRenderNode.h"

class ParticleMeshNode final : public BaseRenderNode {
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

	void preprocess() override {};

private:
	void create_pipeline_state();
};
