#pragma once

#include "Engine/Module/Render/RenderPSO/BaseRenderNode.h"

class ParticleBillboardNode : public BaseRenderNode {
public:
	ParticleBillboardNode();
	~ParticleBillboardNode() noexcept;

	ParticleBillboardNode(const ParticleBillboardNode&) = delete;
	ParticleBillboardNode& operator=(const ParticleBillboardNode&) = delete;
	ParticleBillboardNode(ParticleBillboardNode&&) = default;
	ParticleBillboardNode& operator=(ParticleBillboardNode&&) = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	void preprocess() override {};

private:
	void create_pipeline_state();
};
