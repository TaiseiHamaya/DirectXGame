#pragma once

#include <Engine/Module/Render/RenderPipeline/IPostEffectPipeline.h>

#include <Engine/GraphicsAPI/DirectX/DxResource/ConstantBuffer/ConstantBuffer.h>
#include <Engine/GraphicsAPI/DirectX/DxResource/TextureResource/RenderTexture.h>

/// <summary>
/// Bloom
/// </summary>
namespace szg {

class BloomPipeline : public IPostEffectPipeline {
public:
	struct Data {
		r32 weight;
	};

public:
	BloomPipeline() = default;
	~BloomPipeline() noexcept = default;

	BloomPipeline(const BloomPipeline&) = delete;
	BloomPipeline& operator=(const BloomPipeline&) = delete;
	BloomPipeline(BloomPipeline&&) = default;
	BloomPipeline& operator=(BloomPipeline&&) = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	void preprocess() override {};

	void execute_effect_command() override;

	void set_base_texture(Reference<RenderTexture> baseTexture);
	void set_blur_texture(Reference<RenderTexture> blurTexture);

	Reference<Data> data_mut() noexcept;

private:
	void create_pipeline_state();

#ifdef DEBUG_FEATURES_ENABLE
public:
	void debug_gui();
#endif // DEBUG_FEATURES_ENABLE

private:
	Reference<RenderTexture> baseTexture;
	Reference<RenderTexture> blurTexture;

	ConstantBuffer<Data> data;
};

}; // szg
