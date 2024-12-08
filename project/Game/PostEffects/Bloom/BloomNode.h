#pragma once

#include <Engine/Module/Render/RenderNode/SingleRenderTargetNode.h>

#include "Engine/Rendering/DirectX/DirectXResourceObject/ConstantBuffer/ConstantBuffer.h"

class BloomNode : public SingleRenderTargetNode {
public:
	BloomNode() = default;
	~BloomNode() noexcept = default;

	BloomNode(const BloomNode&) = delete;
	BloomNode& operator=(const BloomNode&) = delete;
	BloomNode(BloomNode&&) = default;
	BloomNode& operator=(BloomNode&&) = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	void draw() const;

	void set_base_texture(const D3D12_GPU_DESCRIPTOR_HANDLE& textureGPUHandle_);
	void set_blur_texture(const D3D12_GPU_DESCRIPTOR_HANDLE& textureGPUHandle_);

private:
	void create_pipeline_state();

#ifdef _DEBUG
public:
	void debug_gui();
#endif // DEBUG

private:
	D3D12_GPU_DESCRIPTOR_HANDLE baseTexture;
	D3D12_GPU_DESCRIPTOR_HANDLE blurTexture;

	struct BloomInfo {
		float weight;
	};
	ConstantBuffer<BloomInfo> bloomInfo;
};
