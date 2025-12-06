#pragma once

#include "Engine/Module/Render/RenderPipeline/IPostEffectPipeline.h"

#include <Library/Math/Vector2.h>

#include "Engine/GraphicsAPI/DirectX/DxResource/ConstantBuffer/ConstantBuffer.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/TextureResource/RenderTexture.h"

namespace szg {

class RadialBlurPipeline : public IPostEffectPipeline {
public:
	struct Data {
		Vector2 center;
		r32 weight;
		r32 length;
		u32 sampleCount;
	};

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

	void preprocess() override {};

	/// <summary>
	/// 描画
	/// </summary>
	void execute_effect_command() override;

	/// <summary>
	/// 描画時に使用するテクスチャリソースを設定
	/// </summary>
	void set_shader_texture(Reference<RenderTexture> baseTexture_);

	Reference<Data> data_mut() noexcept;

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

	ConstantBuffer<Data> data;
};

}; // szg
