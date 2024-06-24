#pragma once

#include "Engine/Render/RenderNode/SingleRenderTargetNode.h"

class SingleRenderTarget;
class SwapChainRenderTargetGroup;
class VertexBuffer;

class GrayscaleNode : public SingleRenderTargetNode {
public:
	GrayscaleNode();
	~GrayscaleNode();

	GrayscaleNode(const GrayscaleNode&) = delete;
	GrayscaleNode& operator=(const GrayscaleNode&) = delete;
	GrayscaleNode(GrayscaleNode&&) = default;
	GrayscaleNode& operator=(GrayscaleNode&&) = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
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
};
