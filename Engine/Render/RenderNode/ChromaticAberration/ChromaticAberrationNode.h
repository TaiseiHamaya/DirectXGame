#pragma once

#include "Engine/Render/RenderNode/SingleRenderTargetNode.h"

#include "Engine/DirectX/DirectXResourceObject/ConstantBuffer/ConstantBuffer.h"

class VertexBuffer;

class ChromaticAberrationNode : public SingleRenderTargetNode {
public:
	ChromaticAberrationNode();
	virtual ~ChromaticAberrationNode() noexcept;

	ChromaticAberrationNode(const ChromaticAberrationNode&) = delete;
	ChromaticAberrationNode& operator=(const ChromaticAberrationNode&) = delete;
	ChromaticAberrationNode(ChromaticAberrationNode&&) = default;
	ChromaticAberrationNode& operator=(ChromaticAberrationNode&&) = default;

public:
	void initialize() override;

	/// <summary>
	/// 描画
	/// </summary>
	void draw();

	/// <summary>
	/// 描画時に使用するテクスチャリソースを設定
	/// </summary>
	/// <param name="textureGPUHandle_">テクスチャのSRVGPUハンドル</param>
	void set_texture_resource(const D3D12_GPU_DESCRIPTOR_HANDLE& textureGPUHandle_);

private:
	/// <summary>
	/// PSO生成
	/// </summary>
	void create_pipline_state();

	/// <summary>
	/// View生成
	/// </summary>
	void create_vertex();

private:
	std::unique_ptr<VertexBuffer> vertex;
	D3D12_GPU_DESCRIPTOR_HANDLE textureGPUHandle;
	ConstantBuffer<float> aberrationLevel;
};
