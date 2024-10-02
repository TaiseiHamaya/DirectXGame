#pragma once

#include "Engine/Render/RenderNode/SingleRenderTargetNode.h"

#include <optional>

class Texture;
class Transform2D;
class VertexBuffer;
class IndexBuffer;

class SpriteNode : public SingleRenderTargetNode {
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

	void use() override;

private:
	/// <summary>
	/// PSO生成
	/// </summary>
	void create_pipeline_state();

private:
	std::optional<D3D12_GPU_DESCRIPTOR_HANDLE> backgroundTextureGPUHandle;
};
