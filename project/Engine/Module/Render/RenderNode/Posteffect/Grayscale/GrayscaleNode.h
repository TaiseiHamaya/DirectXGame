#pragma once

#include "Engine/Module/Render/RenderNode/SingleRenderTargetNode.h"

#include "Engine/GraphicsAPI/DirectX/DxResource/ConstantBuffer/ConstantBuffer.h"

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

private:
	/// <summary>
	/// PSO生成
	/// </summary>
	void create_pipeline_state();

#ifdef _DEBUG
public:
	void debug_gui();
#endif // DEBUG

private:
	D3D12_GPU_DESCRIPTOR_HANDLE textureGPUHandle{};
	ConstantBuffer<std::uint32_t> isGray{};
};
