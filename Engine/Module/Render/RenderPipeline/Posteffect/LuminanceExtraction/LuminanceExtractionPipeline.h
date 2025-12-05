#pragma once

#include <Engine/Module/Render/RenderPipeline/IPostEffectPipeline.h>

#include <Engine/GraphicsAPI/DirectX/DxResource/ConstantBuffer/ConstantBuffer.h>
#include <Engine/GraphicsAPI/DirectX/DxResource/TextureResource/RenderTexture.h>

/// <summary>
/// 色収差
/// </summary>
class LuminanceExtractionPipeline final : public IPostEffectPipeline {
public:
	struct Data {
		r32 intensity;
	};

public:
	LuminanceExtractionPipeline() = default;
	~LuminanceExtractionPipeline() override = default;

	__CLASS_NON_COPYABLE(LuminanceExtractionPipeline)

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	void preprocess() override {};

	void execute_effect_command() override;

public:
	void set_texture_resource(Reference<RenderTexture> baseTexture_);

	Reference<Data> data_mut() noexcept;

private:
	void create_pipeline_state();

private:
	Reference<RenderTexture> baseTexture;

	ConstantBuffer<Data> data;
};
