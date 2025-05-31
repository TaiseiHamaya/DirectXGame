#include "NonLightingPixelNode.h"

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
	gBufferColorTexture->start_read();
	auto& command = DxCommand::GetCommandList();
	gBufferColorTexture->get_as_srv()->use(0);
	command->DrawInstanced(3, 1, 0, 0);
}

void NonLightingPixelNode::set_gbuffers(Reference<RenderTexture> gBufferColorTexture_) {
	gBufferColorTexture = gBufferColorTexture_;
}

void NonLightingPixelNode::create_pipeline_state() {
	RootSignatureBuilder rootSignatureBuilder;
	rootSignatureBuilder.add_texture(D3D12_SHADER_VISIBILITY_PIXEL, 0); // 0 : Albedo

	std::unique_ptr<PSOBuilder> psoBuilder = std::make_unique<PSOBuilder>();
	psoBuilder->blendstate(BlendMode::None);
	psoBuilder->rasterizerstate();
	psoBuilder->rootsignature(rootSignatureBuilder.build());
	psoBuilder->shaders(ShaderType::Vertex, "FullscreenShader.VS.hlsl");
	psoBuilder->shaders(ShaderType::Pixel, "NonLighting.PS.hlsl");
	psoBuilder->primitivetopologytype();
	psoBuilder->rendertarget();

	pipelineState = std::make_unique<DxPipelineState>();
	pipelineState->initialize(psoBuilder->get_rootsignature(), psoBuilder->build());
}
