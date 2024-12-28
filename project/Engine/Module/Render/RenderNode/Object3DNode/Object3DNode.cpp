#include "Object3DNode.h"

#include "Engine/Module/Render/RenderTargetGroup/SingleRenderTarget.h"
#include "Engine/Rendering/DirectX/DirectXResourceObject/DepthStencil/DepthStencil.h"
#include "Engine/Rendering/DirectX/DirectXResourceObject/OffscreenRender/OffscreenRender.h"
#include "Engine/Rendering/DirectX/PipelineState/PipelineState.h"
#include "Engine/Rendering/DirectX/PipelineState/PSOBuilder/PSOBuilder.h"

#include "Engine/Rendering/RenderingSystemValues.h"

Object3DNode::Object3DNode() = default;
Object3DNode::~Object3DNode() noexcept = default;

void Object3DNode::initialize() {
	depthStencil = DepthStencilValue::depthStencil;
	create_pipeline_state();
	pipelineState->set_name("Object3DNode");
	primitiveTopology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

void Object3DNode::set_render_target(const std::shared_ptr<SingleRenderTarget>& renderTarget_) {
	if (renderTarget_) {
		renderTarget = renderTarget_;
		resultSvtHandle = renderTarget_->offscreen_render().texture_gpu_handle();
		renderTarget_->offscreen_render().set_clear_color(RenderingSystemValues::DEFAULT_CLEAR_COLOR);
	}
	else {
		auto temp = std::make_shared<SingleRenderTarget>();
		renderTarget = temp;
		renderTarget->initialize();
		resultSvtHandle = temp->offscreen_render().texture_gpu_handle();
		temp->offscreen_render().set_clear_color(RenderingSystemValues::DEFAULT_CLEAR_COLOR);
	}
}

void Object3DNode::create_pipeline_state() {
	RootSignatureBuilder rootSignatureBuilder;
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_VERTEX, 0); // 0 :  transform
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_VERTEX, 1); // 1 : camera
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_PIXEL, 0); // 2 : material
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_PIXEL, 1); // 3 : light
	rootSignatureBuilder.add_texture(D3D12_SHADER_VISIBILITY_PIXEL); // 4 : texture
	rootSignatureBuilder.sampler( // sampler
		D3D12_SHADER_VISIBILITY_PIXEL,
		0,
		D3D12_FILTER_ANISOTROPIC
	);

	InputLayoutBuilder inputLayoutBuilder;
	inputLayoutBuilder.add_element("POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT);
	inputLayoutBuilder.add_element("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT);
	inputLayoutBuilder.add_element("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT);

	ShaderBuilder shaderBuilder;
	shaderBuilder.initialize(
		"EngineResources/HLSL/3DObject/Object3d.VS.hlsl",
		"EngineResources/HLSL/3DObject/Object3d.PS.hlsl"
	);

	std::unique_ptr<PSOBuilder> psoBuilder = std::make_unique<PSOBuilder>();
	psoBuilder->blendstate();
	psoBuilder->depthstencilstate(*depthStencil);
	psoBuilder->inputlayout(inputLayoutBuilder.build());
	psoBuilder->rasterizerstate();
	psoBuilder->rootsignature(rootSignatureBuilder.build());
	psoBuilder->shaders(shaderBuilder);
	psoBuilder->primitivetopologytype();
	psoBuilder->rendertarget();

	pipelineState = std::make_unique<PipelineState>();
	pipelineState->initialize(psoBuilder->get_rootsignature(), psoBuilder->build());
}
