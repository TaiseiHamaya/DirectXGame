#include "ObjectDepth3DNode.h"

#include <d3d12.h>
#include <cassert>

#include "Engine/DirectX/DirectXCore.h"
#include "Engine/DirectX/DirectXSwapChain/DirectXSwapChain.h"
#include "Engine/DirectX/PipelineState/PipelineState.h"
#include "Engine/DirectX/PipelineState/PSOBuilder/PSOBuilder.h"
#include "Engine/Render/RenderTargetGroup/SingleRenderTarget.h"
#include "Engine/DirectX/DirectXResourceObject/OffscreenRender/OffscreenRender.h"
#include "Engine/Render/RenderTargetGroup/MultiRenderTarget.h"

ObjectDepth3DNode::ObjectDepth3DNode() = default;

ObjectDepth3DNode::~ObjectDepth3DNode() noexcept = default;

void ObjectDepth3DNode::initialize() {
	create_pipeline_state();
	primitiveTopology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

void ObjectDepth3DNode::begin() {
	BaseRenderNode::begin();
	DirectXCore::Set3DLight();
}

void ObjectDepth3DNode::set_render_target(const std::shared_ptr<MultiRenderTarget>& renderTarget_) {
	if (renderTarget_) {
		renderTarget = renderTarget_;
		auto&& renderList = renderTarget_->offscreen_render_list();
		for (int i = 0; i < renderList.size(); ++i) {
			resultSrvHandleList.emplace_back(renderList[i].texture_gpu_handle());
		}
	}
	else {
		auto temp = std::make_shared<MultiRenderTarget>();
		temp->initialize(2);
		renderTarget = temp;
		auto&& renderList = temp->offscreen_render_list();
		renderList[1].release_index();
		renderList[1].initialize(WinApp::GetClientWidth(), WinApp::GetClientHight(), DXGI_FORMAT_R32_FLOAT);
		renderTarget->set_depth_stencil(DirectXSwapChain::GetDepthStencil());
		for (auto itr = renderList.begin(); itr != renderList.end(); ++itr) {
			resultSrvHandleList.emplace_back(itr->texture_gpu_handle());
		}
	}
	renderTarget->set_clear_color({ 1.0f,1.0f,1.0f,1.0f });
	assert(renderTarget->has_depth());
}

void ObjectDepth3DNode::create_pipeline_state() {
	RootSignatureBuilder rootSignatureBuilder;
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_VERTEX, 0);
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_PIXEL, 0);
	rootSignatureBuilder.add_texture(D3D12_SHADER_VISIBILITY_PIXEL);
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_PIXEL, 1);
	rootSignatureBuilder.sampler(
		D3D12_SHADER_VISIBILITY_PIXEL,
		0,
		D3D12_FILTER_ANISOTROPIC
	);

	InputLayoutBuilder inputLayoutBuillder;
	inputLayoutBuillder.add_element("POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT);
	inputLayoutBuillder.add_element("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT);
	inputLayoutBuillder.add_element("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT);

	ShaderBuilder shaderManager;
	shaderManager.initialize(
		"Engine/Render/RenderNode/Object3DNode/ObjectDepth3DNode/ObjectDepth3D.VS.hlsl",
		"Engine/Render/RenderNode/Object3DNode/ObjectDepth3DNode/ObjectDepth3D.PS.hlsl"
	);

	std::unique_ptr<PSOBuilder> psoBuilder = std::make_unique<PSOBuilder>();
	psoBuilder->blendstate(BlendMode::None, 0);
	//psoBuilder->blendstate(BlendMode::None, 1);
	psoBuilder->depthstencilstate(DirectXSwapChain::GetDepthStencil()->get_desc());
	psoBuilder->inputlayout(inputLayoutBuillder.build());
	psoBuilder->rasterizerstate();
	psoBuilder->rootsignature(rootSignatureBuilder.build());
	psoBuilder->shaders(shaderManager);
	psoBuilder->primitivetopologytype();
	psoBuilder->rendertarget();
	psoBuilder->rendertarget(DXGI_FORMAT_R32_FLOAT);

	pipelineState = std::make_unique<PipelineState>();
	pipelineState->initialize(psoBuilder->get_rootsignature(), psoBuilder->build());
}
