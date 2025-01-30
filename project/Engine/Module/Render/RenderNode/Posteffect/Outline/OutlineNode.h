#pragma once

#include "Engine/Module/Render/RenderNode/SingleRenderTargetNode.h"

class OutlineNode : public SingleRenderTargetNode {
public:
	OutlineNode();
	~OutlineNode();

	OutlineNode(const OutlineNode&) = delete;
	OutlineNode& operator=(const OutlineNode&) = delete;
	OutlineNode(OutlineNode&&) = default;
	OutlineNode& operator=(OutlineNode&&) = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	void preprocess() override {};

	/// <summary>
	/// 描画
	/// </summary>
	void draw();

	/// <summary>
	/// 描画時に使用するテクスチャリソースを設定
	/// </summary>
	/// <param name="textureGPUHandle_">テクスチャのSRVGPUハンドル</param>
	void set_texture_resource(const D3D12_GPU_DESCRIPTOR_HANDLE& textureGPUHandle_);

	void set_depth_resource(const D3D12_GPU_DESCRIPTOR_HANDLE& depthGPUHandle_);

private:
	/// <summary>
	/// PSO生成
	/// </summary>
	void create_pipeline_state();

private:
	D3D12_GPU_DESCRIPTOR_HANDLE textureGPUHandle{};
	D3D12_GPU_DESCRIPTOR_HANDLE depthGPUHandle{};
};
