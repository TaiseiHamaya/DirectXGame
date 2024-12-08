#pragma once

#include <Engine/Module/Render/RenderNode/SingleRenderTargetNode.h>

#include <Engine/Rendering/DirectX/DirectXResourceObject/ConstantBuffer/ConstantBuffer.h>

class GaussianBlurNode : public SingleRenderTargetNode {
public:
	GaussianBlurNode() = default;
	~GaussianBlurNode() noexcept = default;

	GaussianBlurNode(const GaussianBlurNode&) = delete;
	GaussianBlurNode& operator=(const GaussianBlurNode&) = delete;
	GaussianBlurNode(GaussianBlurNode&&) = default;
	GaussianBlurNode& operator=(GaussianBlurNode&&) = default;

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
#endif // _DEBUG

private:
	D3D12_GPU_DESCRIPTOR_HANDLE textureGPUHandle;

	struct GaussianBlurInfo {
		float dispersion;
		float length;
		uint32_t sampleCount;
	};
	ConstantBuffer<GaussianBlurInfo> blurInfo;
};
