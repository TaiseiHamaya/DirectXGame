#pragma once

#include <Engine/Render/RenderNode/SingleRenderTargetNode.h>

class ParticleNode final : public SingleRenderTargetNode {
public:
	ParticleNode();
	~ParticleNode() noexcept;

	ParticleNode(const ParticleNode&) = delete;
	ParticleNode& operator=(const ParticleNode&) = delete;
	ParticleNode(ParticleNode&&) = default;
	ParticleNode& operator=(ParticleNode&&) = default;

public:
	void initialize() override;

	void use() override;

private:
	void create_pipeline_state();
};
