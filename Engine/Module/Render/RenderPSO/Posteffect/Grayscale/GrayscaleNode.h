#pragma once

#include "Engine/Module/Render/RenderPSO/PostEffectPSO.h"

#include <Library/Utility/Template/Reference.h>

#include "Engine/GraphicsAPI/DirectX/DxResource/ConstantBuffer/ConstantBuffer.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/TextureResource/RenderTexture.h"

class GrayscaleNode : public PostEffectPSO {
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
	ConstantBuffer<u32> isGray{};
};
