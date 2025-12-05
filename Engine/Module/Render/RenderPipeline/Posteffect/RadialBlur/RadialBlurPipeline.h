#pragma once

#include "Engine/Module/Render/RenderPipeline/IPostEffectPipeline.h"

#include <Library/Math/Vector2.h>

#include "Engine/GraphicsAPI/DirectX/DxResource/ConstantBuffer/ConstantBuffer.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/TextureResource/RenderTexture.h"

struct BlurInfo {
	Vector2 center;
	r32 weight;
	r32 length;
	u32 sampleCount;
};

class RadialBlurPipeline : public IPostEffectPipeline {
public:
	RadialBlurPipeline();
	~RadialBlurPipeline() noexcept;

	RadialBlurPipeline(const RadialBlurPipeline&) = delete;
	RadialBlurPipeline& operator=(const RadialBlurPipeline&) = delete;
	RadialBlurPipeline(RadialBlurPipeline&&) = default;
	RadialBlurPipeline& operator=(RadialBlurPipeline&&) = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	void preprocess() override;

	/// <summary>
	/// 描画
	/// </summary>
	void execute_effect_command() override;

	/// <summary>
	/// 描画時に使用するテクスチャリソースを設定
	/// </summary>
	void set_shader_texture(Reference<RenderTexture> baseTexture_);

	BlurInfo& data() { return *blurInfo.data_mut(); }

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
