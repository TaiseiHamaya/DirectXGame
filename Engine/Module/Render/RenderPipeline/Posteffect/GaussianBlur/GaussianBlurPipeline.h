#pragma once

#include <Engine/Module/Render/RenderPipeline/IPostEffectPipeline.h>

#include <Engine/GraphicsAPI/DirectX/DxResource/ConstantBuffer/ConstantBuffer.h>
#include <Engine/GraphicsAPI/DirectX/DxResource/TextureResource/RenderTexture.h>

/// <summary>
/// GaussianBlur
/// </summary>
namespace szg {

class GaussianBlurPipeline : public IPostEffectPipeline {
public:
	struct Data {
		r32 dispersion;
		r32 length;
		u32 sampleCount;
	};

public:
	GaussianBlurPipeline() = default;
	~GaussianBlurPipeline() noexcept = default;

	GaussianBlurPipeline(const GaussianBlurPipeline&) = delete;
	GaussianBlurPipeline& operator=(const GaussianBlurPipeline&) = delete;
	GaussianBlurPipeline(GaussianBlurPipeline&&) = default;
	GaussianBlurPipeline& operator=(GaussianBlurPipeline&&) = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	void preprocess() override {};

	void execute_effect_command() override;

	void set_base_texture(Reference<RenderTexture> baseTexture_);

public:
	void set_parameters(r32 dispersion, r32 length, u32 sampleCount);

	Reference<Data> data_mut() noexcept;

private:
	void create_pipeline_state();

#ifdef DEBUG_FEATURES_ENABLE
public:
	void debug_gui();
#endif // DEBUG_FEATURES_ENABLE

private:
	Reference<RenderTexture> baseTexture;

	ConstantBuffer<Data> data;
};

}; // szg
