#pragma once

#include "Engine/Render/RenderNode/SingleRenderTargetNode.h"

#include <optional>
#include <memory>

#include "Engine/Math/Matrix4x4.h"
#include "Engine/Game/Color/Color.h"
#include "Engine/DirectX/DirectXResourceObject/ConstantBuffer/ConstantBuffer.h"

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

	void begin() override;

	void set_background_texture(const D3D12_GPU_DESCRIPTOR_HANDLE& textureGPUHandle_);

private:
	/// <summary>
	/// PSO生成
	/// </summary>
	void create_pipeline_state();

	void create_background_draw_data();

private:
	std::optional<D3D12_GPU_DESCRIPTOR_HANDLE> backgroundTextureGPUHandle;

	std::unique_ptr<VertexBuffer> vertices;
	std::unique_ptr<IndexBuffer> indexes;

	struct SpriteMaterial {
		Color color;
		Matrix4x4 uvTransform;
	};

	std::unique_ptr<ConstantBuffer<SpriteMaterial>> material;
	std::unique_ptr<ConstantBuffer<Matrix4x4>> transformMatrix;
};
