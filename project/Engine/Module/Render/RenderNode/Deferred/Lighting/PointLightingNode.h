#pragma once

#include "Engine/Module/Render/RenderNode/SingleRenderTargetNode.h"

#include "../DeferredAdaptor.h"

/// 既知の不具合一覧 
/// ・ライト内にカメラがあった際に、正しくライティングされない。
/// ・ライト位置に対して背面を向いている面でライティングされる場合がある。

class PointLightingNode final : public SingleRenderTargetNode {
public:
	PointLightingNode();
	~PointLightingNode() noexcept;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	void set_gbuffers(std::shared_ptr<DeferredAdaptor::GBuffersType> gBufferRT);

	void preprocess() override;

private:
	void create_pipeline_state();

private:
	std::array<D3D12_GPU_DESCRIPTOR_HANDLE, DeferredAdaptor::NUM_GBUFFER> gBuffers{ 0 };
	D3D12_GPU_DESCRIPTOR_HANDLE depthBuffer{ 0 };
};

