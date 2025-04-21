#include "NonLightingPixelNode.h"

#include "Engine/GraphicsAPI/DirectX/DxResource/OffscreenRender/OffscreenRender.h"
#include "Engine/GraphicsAPI/DirectX/DxPipelineState/DxPipelineState.h"
#include "Engine/GraphicsAPI/DirectX/DxPipelineState/PSOBuilder/PSOBuilder.h"
#include "Engine/Module/Render/RenderTargetGroup/MultiRenderTarget.h"

NonLightingPixelNode::NonLightingPixelNode() = default;
NonLightingPixelNode::~NonLightingPixelNode() noexcept = default;

void NonLightingPixelNode::initialize() {
	create_pipeline_state();
	pipelineState->set_name("NonLightingPixelNode");
	primitiveTopology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

void NonLightingPixelNode::preprocess() {
	auto& command = DxCommand::GetCommandList();
	command->SetGraphicsRootDescriptorTable(0, gBuffer);
	command->DrawInstanced(3, 1, 0, 0);
}

void NonLightingPixelNode::set_gbuffers(std::shared_ptr<DeferredAdaptor::GBuffersType> gBufferRT) {
	auto& list = gBufferRT->offscreen_render_list();
	gBuffer = list[0].texture_gpu_handle();
}

void NonLightingPixelNode::create_pipeline_state() {
	RootSignatureBuilder rootSignatureBuilder;
	rootSignatureBuilder.add_texture(D3D12_SHADER_VISIBILITY_PIXEL, 0); // 0 : Albedo

	ShaderBuilder shaderBuilder;
	shaderBuilder.initialize(
		"EngineResources/HLSL/FullscreenShader.VS.hlsl",
		"EngineResources/HLSL/Deferred/Lighting/NonLighting.PS.hlsl"
	);

	std::unique_ptr<PSOBuilder> psoBuilder = std::make_unique<PSOBuilder>();
	psoBuilder->blendstate(BlendMode::None);
	psoBuilder->rasterizerstate();
	psoBuilder->rootsignature(rootSignatureBuilder.build());
	psoBuilder->shaders(shaderBuilder);
	psoBuilder->primitivetopologytype();
	psoBuilder->rendertarget();

	pipelineState = std::make_unique<DxPipelineState>();
	pipelineState->initialize(psoBuilder->get_rootsignature(), psoBuilder->build());
}
