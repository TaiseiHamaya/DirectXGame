#pragma once

#include "Engine/Module/Render/RenderPSO/BaseRenderNode.h"

class SpriteNode : public BaseRenderNode {
public:
	SpriteNode();
	~SpriteNode() noexcept;

	SpriteNode(const SpriteNode&) = delete;
	SpriteNode& operator=(const SpriteNode&) = delete;
	SpriteNode(SpriteNode&&) = default;
	SpriteNode& operator=(SpriteNode&&) = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	void preprocess() override {};

private:
	/// <summary>
	/// PSO生成
	/// </summary>
	void create_pipeline_state();
};
