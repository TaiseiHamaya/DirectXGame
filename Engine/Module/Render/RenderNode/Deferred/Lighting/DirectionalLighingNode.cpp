#include "DirectionalLighingNode.h"

#include "Engine/GraphicsAPI/DirectX/DxResource/DepthStencil/DepthStencil.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/OffscreenRender/OffscreenRender.h"
#include "Engine/GraphicsAPI/DirectX/DxPipelineState/DxPipelineState.h"
#include "Engine/GraphicsAPI/DirectX/DxPipelineState/PSOBuilder/PSOBuilder.h"
#include "Engine/Module/Render/RenderTargetGroup/MultiRenderTarget.h"

DirectionalLightingNode::DirectionalLightingNode() = default;

DirectionalLightingNode::~DirectionalLightingNode() noexcept = default;

void DirectionalLightingNode::initialize() {
	create_pipeline_state();
	pipelineState->set_name("DirectionalLightingNode");
	primitiveTopology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	depthBuffer = DepthStencilValue::depthStencil->texture_gpu_handle();
}

void DirectionalLightingNode::preprocess() {
	auto& command = DxCommand::GetCommandList();
	for (uint32_t i = 0; i < DeferredAdaptor::NUM_GBUFFER; ++i) {
		command->SetGraphicsRootDescriptorTable(2 + i, gBuffers[i]);
	}
	command->SetGraphicsRootDescriptorTable(4, depthBuffer);
}

void DirectionalLightingNode::set_gbuffers(std::shared_ptr<DeferredAdaptor::GBuffersType> gBufferRT) {
	auto& list = gBufferRT->offscreen_render_list();
	for (uint32_t i = 0; i < DeferredAdaptor::NUM_GBUFFER; ++i) {
		gBuffers[i] = list[i].texture_gpu_handle();
	}
}

void DirectionalLightingNode::create_pipeline_state() {
	RootSignatureBuilder rootSignatureBuilder;
	rootSignatureBuilder.add_structured(D3D12_SHADER_VISIBILITY_PIXEL, 3); // 0 : Light
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_PIXEL, 0); // 1 : Camera
	rootSignatureBuilder.add_texture(D3D12_SHADER_VISIBILITY_PIXEL, 0); // 2 : Albedo
	rootSignatureBuilder.add_texture(D3D12_SHADER_VISIBILITY_PIXEL, 1); // 3 : Normal
	rootSignatureBuilder.add_texture(D3D12_SHADER_VISIBILITY_PIXEL, 2); // 4 : Depth

	ShaderBuilder shaderBuilder;
	shaderBuilder.initialize(
		"DirectXGame/EngineResources/HLSL/FullscreenShader.VS.hlsl",
		"DirectXGame/EngineResources/HLSL/Deferred/Lighting/DirectionalLighting.PS.hlsl"
	);

	std::unique_ptr<PSOBuilder> psoBuilder = std::make_unique<PSOBuilder>();
	psoBuilder->blendstate(BlendMode::LightingPath);
	psoBuilder->rasterizerstate();
	psoBuilder->rootsignature(rootSignatureBuilder.build());
	psoBuilder->shaders(shaderBuilder);
	psoBuilder->primitivetopologytype();
	psoBuilder->rendertarget();

	pipelineState = std::make_unique<DxPipelineState>();
	pipelineState->initialize(psoBuilder->get_rootsignature(), psoBuilder->build());
}
