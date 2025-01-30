#pragma once

#include "Engine/Module/Render/RenderNode/SingleRenderTargetNode.h"

#include <Library/Math/Vector2.h>

#include "Engine/GraphicsAPI/DirectX/DxResource/ConstantBuffer/ConstantBuffer.h"

class RadialBlurNode : public SingleRenderTargetNode {
public:
	RadialBlurNode();
	~RadialBlurNode() noexcept;

	RadialBlurNode(const RadialBlurNode&) = delete;
	RadialBlurNode& operator=(const RadialBlurNode&) = delete;
	RadialBlurNode(RadialBlurNode&&) = default;
	RadialBlurNode& operator=(RadialBlurNode&&) = default;

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
	D3D12_GPU_DESCRIPTOR_HANDLE textureGPUHandle;

	struct BlurInfo {
		Vector2 center;
		float weight;
		float length;
		std::uint32_t sampleCount;
	};
	ConstantBuffer<BlurInfo> blurInfo;
};
