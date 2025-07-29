#pragma once

#include "Engine/Module/Render/RenderNode/SingleRenderTargetNode.h"

#include <Library/Math/Vector2.h>

#include "Engine/GraphicsAPI/DirectX/DxResource/ConstantBuffer/ConstantBuffer.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/TextureResource/RenderTexture.h"

struct BlurInfo {
	Vector2 center;
	r32 weight;
	r32 length;
	u32 sampleCount;
};

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
	void set_texture_resource(Reference<RenderTexture> baseTexture_);

	BlurInfo& data() { return *blurInfo.get_data(); }

private:
	/// <summary>
	/// PSO生成
	/// </summary>
	void create_pipeline_state();

#ifdef DEBUG_FEATURES_ENABLE
public:
	void debug_gui();
#endif // DEBUG

private:
	Reference<RenderTexture> baseTexture;

	ConstantBuffer<BlurInfo> blurInfo;
};
