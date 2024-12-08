#pragma once

#include <Engine/Module/Render/RenderNode/SingleRenderTargetNode.h>

#include "Engine/Rendering/DirectX/DirectXResourceObject/ConstantBuffer/ConstantBuffer.h"

class LuminanceExtractionNode : public SingleRenderTargetNode {
public:
	LuminanceExtractionNode() = default;
	~LuminanceExtractionNode() noexcept = default;

	LuminanceExtractionNode(const LuminanceExtractionNode&) = delete;
	LuminanceExtractionNode& operator=(const LuminanceExtractionNode&) = delete;
	LuminanceExtractionNode(LuminanceExtractionNode&&) = default;
	LuminanceExtractionNode& operator=(LuminanceExtractionNode&&) = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	void draw() const;

	/// <summary>
	/// 描画時に使用するテクスチャリソースを設定
	/// </summary>
	/// <param name="textureGPUHandle_">テクスチャのSRVGPUハンドル</param>
	void set_texture_resource(const D3D12_GPU_DESCRIPTOR_HANDLE& textureGPUHandle_);

	void set_render_target(const std::shared_ptr<SingleRenderTarget>& renderTarget_ = nullptr) override;

private:
	void create_pipeline_state();

#ifdef _DEBUG
public:
	void debug_gui();
#endif // DEBUG

private:
	D3D12_GPU_DESCRIPTOR_HANDLE textureGPUHandle;

	struct LuminanceExtractionInfo {
		float intensity;
	};
	ConstantBuffer<LuminanceExtractionInfo> luminanceExtractionInfo;
};
