#pragma once

#include "Engine/Module/Render/RenderPSO/PostEffectPSO.h"

#include <Library/Utility/Template/Reference.h>

class RenderTexture;
class DepthStencilTexture;

class OutlineNode : public PostEffectPSO {
public:
	OutlineNode();
	~OutlineNode();

	OutlineNode(const OutlineNode&) = delete;
	OutlineNode& operator=(const OutlineNode&) = delete;
	OutlineNode(OutlineNode&&) = default;
	OutlineNode& operator=(OutlineNode&&) = default;

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
	/// <param name="textureGPUHandle_">テクスチャのSRVGPUハンドル</param>
	void set_shader_texture(Reference<RenderTexture> baseTexture_, Reference<DepthStencilTexture> depthTexture_);

private:
	/// <summary>
	/// PSO生成
	/// </summary>
	void create_pipeline_state();

private:
	Reference<RenderTexture> baseTexture;
	Reference<DepthStencilTexture> depthTexture;
};
