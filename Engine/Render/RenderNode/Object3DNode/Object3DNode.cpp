#include "Object3DNode.h"

#include <cassert>

#include "Engine/DirectX/DirectXCore.h"
#include "Engine/DirectX/DirectXSwapChain/DirectXSwapChain.h"
#include "Engine/DirectX/PipelineState/PipelineState.h"
#include "Engine/DirectX/PipelineState/PSOBuilder/PSOBuilder.h"
#include "Engine/Render/RenderTargetGroup/SingleRenderTarget.h"
#include "Engine/Render/RenderTargetGroup/SwapChainRenderTargetGroup.h"
#include "Engine/DirectX/DirectXResourceObject/OffscreenRender/OffscreenRender.h"

Object3DNode::Object3DNode() = default;
Object3DNode::~Object3DNode() noexcept = default;

void Object3DNode::initialize() {
	create_pipline_state();
	primitiveTopology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

void Object3DNode::set_render_target(const std::shared_ptr<SingleRenderTarget>& renderTarget_) {
	if (renderTarget_) {
		renderTarget = renderTarget_;
		resultSvtHandle = renderTarget_->main_offscreen_render().texture_gpu_handle();
	}
	else {
		auto temp = std::make_shared<SingleRenderTarget>();
		renderTarget = temp;
		renderTarget->initialize();
		renderTarget->set_depth_stencil(DirectXSwapChain::GetDepthStencil());
		resultSvtHandle = temp->main_offscreen_render().texture_gpu_handle();
	}
	assert(renderTarget->has_depth());
}

void Object3DNode::begin() {
	BaseRenderNode::begin();
	DirectXCore::Set3DLight();
}

void Object3DNode::create_pipline_state() {
	RootSignatureBuilder rootSignatureBuilder;
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_VERTEX, 0);
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_PIXEL, 0);
	rootSignatureBuilder.descriptor_range();
	rootSignatureBuilder.add_texture(D3D12_SHADER_VISIBILITY_PIXEL);
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_PIXEL, 1);
	rootSignatureBuilder.sampler(
		D3D12_SHADER_VISIBILITY_PIXEL,
		0
	);

	InputLayoutBuillder inputLayoutBuillder;
	inputLayoutBuillder.add_cbv("POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT);
	inputLayoutBuillder.add_cbv("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT);
	inputLayoutBuillder.add_cbv("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT);

	ShaderBuilder shaderManager;
	shaderManager.initialize();

	std::unique_ptr<PSOBuilder> psoBuilder = std::make_unique<PSOBuilder>();
	psoBuilder->blendstate();
	psoBuilder->depthstencilstate(DirectXSwapChain::GetDepthStencil()->get_desc());
	psoBuilder->inputlayout(inputLayoutBuillder.build());
	psoBuilder->rasterizerstate();
	psoBuilder->rootsignature(rootSignatureBuilder.build());
	psoBuilder->shaders(shaderManager);
	psoBuilder->primitivetopologytype();
	pipelineState = std::make_unique<PipelineState>();
	pipelineState->initialize(psoBuilder->get_rootsignature(), psoBuilder->build());
}
