#pragma once

#include "Engine/Module/Render/RenderNode/SingleRenderTargetNode.h"

#include "../DeferredAdaptor.h"

class NonLightingPixelNode final : public SingleRenderTargetNode {
public:
	NonLightingPixelNode();
	~NonLightingPixelNode() noexcept;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	void preprocess() override;

	void set_gbuffers(std::shared_ptr<DeferredAdaptor::GBuffersType> gBufferRT);

private:
	void create_pipeline_state();

private:
	D3D12_GPU_DESCRIPTOR_HANDLE gBuffer{ 0 };
};
