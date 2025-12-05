#pragma once

#include <Engine/Module/Render/RenderPipeline/IPostEffectPipeline.h>

#include <Engine/GraphicsAPI/DirectX/DxResource/ConstantBuffer/ConstantBuffer.h>
#include <Engine/GraphicsAPI/DirectX/DxResource/TextureResource/RenderTexture.h>

/// <summary>
/// 色収差
/// </summary>
class LuminanceExtractionPipeline : public IPostEffectPipeline {
public:
	LuminanceExtractionPipeline() = default;
	~LuminanceExtractionPipeline() noexcept = default;

	LuminanceExtractionPipeline(const LuminanceExtractionPipeline&) = delete;
	LuminanceExtractionPipeline& operator=(const LuminanceExtractionPipeline&) = delete;
	LuminanceExtractionPipeline(LuminanceExtractionPipeline&&) = default;
	LuminanceExtractionPipeline& operator=(LuminanceExtractionPipeline&&) = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	void preprocess() override {};

	void execute_effect_command() const;

	void set_texture_resource(Reference<RenderTexture> baseTexture_);

private:
	void create_pipeline_state();

#ifdef DEBUG_FEATURES_ENABLE
public:
	void debug_gui();
#endif // DEBUG_FEATURES_ENABLE

private:
	Reference<RenderTexture> baseTexture;

	struct LuminanceExtractionInfo {
		r32 intensity;
	};
	ConstantBuffer<LuminanceExtractionInfo> luminanceExtractionInfo;
};
