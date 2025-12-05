#pragma once

#include "Engine/Module/Render/RenderPipeline/IPostEffectPipeline.h"

#include <Library/Math/Vector2.h>

#include "Engine/GraphicsAPI/DirectX/DxResource/ConstantBuffer/ConstantBuffer.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/TextureResource/RenderTexture.h"

class ChromaticAberrationPipeline : public IPostEffectPipeline {
public:
	ChromaticAberrationPipeline();
	virtual ~ChromaticAberrationPipeline() noexcept;

	ChromaticAberrationPipeline(const ChromaticAberrationPipeline&) = delete;
	ChromaticAberrationPipeline& operator=(const ChromaticAberrationPipeline&) = delete;
	ChromaticAberrationPipeline(ChromaticAberrationPipeline&&) = default;
	ChromaticAberrationPipeline& operator=(ChromaticAberrationPipeline&&) = default;

public:
	void initialize() override;

	void preprocess() override;

	/// <summary>
	/// 描画
	/// </summary>
	void execute_effect_command();

	/// <summary>
	/// 描画時に使用するテクスチャリソースを設定
	/// </summary>
	void set_shader_texture(Reference<RenderTexture> baseTexture_);

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
	ConstantBuffer<Vector2> aberrationLevel{};
};
