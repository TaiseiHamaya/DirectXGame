#include "PointLightingNode.h"

#include "Engine/Rendering/DirectX/DirectXResourceObject/DepthStencil/DepthStencil.h"
#include "Engine/Rendering/DirectX/DirectXResourceObject/OffscreenRender/OffscreenRender.h"
#include "Engine/Rendering/DirectX/PipelineState/PipelineState.h"
#include "Engine/Rendering/DirectX/PipelineState/PSOBuilder/PSOBuilder.h"
#include <Engine/Module/Render/RenderTargetGroup/MultiRenderTarget.h>

PointLightingNode::PointLightingNode() = default;

PointLightingNode::~PointLightingNode() noexcept = default;

void PointLightingNode::initialize() {
	create_pipeline_state();
	pipelineState->set_name("PointLightingNode");
	primitiveTopology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	depthBuffer = DepthStencilValue::depthStencil->texture_gpu_handle();
}

void PointLightingNode::preprocess() {
	auto& command = DirectXCommand::GetCommandList();
	for (uint32_t i = 0; i < DeferredAdaptor::NUM_GBUFFER; ++i) {
		command->SetGraphicsRootDescriptorTable(2 + i, gBuffers[i]);
	}
	command->SetGraphicsRootDescriptorTable(4, depthBuffer);
}

void PointLightingNode::set_gbuffers(std::shared_ptr<DeferredAdaptor::GBuffersType> gBufferRT) {
	auto& list = gBufferRT->offscreen_render_list();
	for (uint32_t i = 0; i < DeferredAdaptor::NUM_GBUFFER; ++i) {
		gBuffers[i] = list[i].texture_gpu_handle();
	}
}

void PointLightingNode::create_pipeline_state() {
	RootSignatureBuilder rootSignatureBuilder;
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_PIXEL, 0); // 0 : Light
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_PIXEL, 1); // 1 : Camera
	rootSignatureBuilder.add_texture(D3D12_SHADER_VISIBILITY_PIXEL, 0); // 2 : Albedo
	rootSignatureBuilder.add_texture(D3D12_SHADER_VISIBILITY_PIXEL, 1); // 3 : Normal
	rootSignatureBuilder.add_texture(D3D12_SHADER_VISIBILITY_PIXEL, 2); // 4 : Depth
	rootSignatureBuilder.sampler( // sampler
		D3D12_SHADER_VISIBILITY_PIXEL, 0
	);

	ShaderBuilder shaderBuilder;
	shaderBuilder.initialize(
		"EngineResources/HLSL/.hlsl",
		"EngineResources/HLSL/Deferred/Lighting/PointLighting.hlsl"
	);

	std::unique_ptr<PSOBuilder> psoBuilder = std::make_unique<PSOBuilder>();
	psoBuilder->blendstate(BlendMode::LightingPath);
	psoBuilder->rasterizerstate();
	psoBuilder->rootsignature(rootSignatureBuilder.build());
	psoBuilder->shaders(shaderBuilder);
	psoBuilder->primitivetopologytype();
	psoBuilder->rendertarget();

	pipelineState = std::make_unique<PipelineState>();
	pipelineState->initialize(psoBuilder->get_rootsignature(), psoBuilder->build());
}
