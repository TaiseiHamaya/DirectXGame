#include "DirectionalLighingNode.h"

#include "Engine/GraphicsAPI/DirectX/DxPipelineState/DxPipelineState.h"
#include "Engine/GraphicsAPI/DirectX/DxPipelineState/PSOBuilder/PSOBuilder.h"
#include "Engine/GraphicsAPI/RenderingSystemValues.h"
#include "Engine/Module/Render/RenderTargetGroup/MultiRenderTarget.h"

DirectionalLightingNode::DirectionalLightingNode() = default;

DirectionalLightingNode::~DirectionalLightingNode() noexcept = default;

void DirectionalLightingNode::initialize() {
	create_pipeline_state();
	pipelineState->set_name("DirectionalLightingNode");
	primitiveTopology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	depthTexture = RenderingSystemValues::GetDepthStencilTexture();
}

void DirectionalLightingNode::preprocess() {
	auto& command = DxCommand::GetCommandList();
	for (u32 i = 0; i < DeferredAdaptor::NUM_GBUFFER; ++i) {
		gBufferTextures[i]->start_read();
		gBufferTextures[i]->get_as_srv()->use(i + 2);
	}
	depthTexture->start_read();
	depthTexture->get_as_srv()->use(4);
}

void DirectionalLightingNode::set_gbuffers(std::array<Reference<RenderTexture>, DeferredAdaptor::NUM_GBUFFER> gBufferTextures_) {
	gBufferTextures = gBufferTextures_;
}

void DirectionalLightingNode::create_pipeline_state() {
	RootSignatureBuilder rootSignatureBuilder;
	rootSignatureBuilder.add_structured(D3D12_SHADER_VISIBILITY_PIXEL, 3); // 0 : Light
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_PIXEL, 0); // 1 : Camera
	rootSignatureBuilder.add_texture(D3D12_SHADER_VISIBILITY_PIXEL, 0); // 2 : Albedo
	rootSignatureBuilder.add_texture(D3D12_SHADER_VISIBILITY_PIXEL, 1); // 3 : Normal
	rootSignatureBuilder.add_texture(D3D12_SHADER_VISIBILITY_PIXEL, 2); // 4 : Depth

	std::unique_ptr<PSOBuilder> psoBuilder = std::make_unique<PSOBuilder>();
	psoBuilder->blendstate(BlendMode::LightingPath);
	psoBuilder->rasterizerstate();
	psoBuilder->rootsignature(rootSignatureBuilder.build());
	psoBuilder->shaders(ShaderType::Vertex, "FullscreenShader.VS.hlsl");
	psoBuilder->shaders(ShaderType::Pixel, "DirectionalLighting.PS.hlsl");
	psoBuilder->primitivetopologytype();
	psoBuilder->rendertarget();

	pipelineState = std::make_unique<DxPipelineState>();
	pipelineState->initialize(psoBuilder->get_rootsignature(), psoBuilder->build());
}
