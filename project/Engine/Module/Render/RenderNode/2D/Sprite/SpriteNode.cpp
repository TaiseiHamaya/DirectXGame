#include "SpriteNode.h"

#include "Engine/GraphicsAPI/DirectX/PipelineState/PipelineState.h"
#include "Engine/GraphicsAPI/DirectX/PipelineState/PSOBuilder/PSOBuilder.h"

SpriteNode::SpriteNode() = default;

SpriteNode::~SpriteNode() noexcept = default;

void SpriteNode::initialize() {
	create_pipeline_state();
	primitiveTopology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

void SpriteNode::create_pipeline_state() {
	RootSignatureBuilder rootSignatureBuilder;
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_VERTEX, 0);
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_PIXEL, 0);
	rootSignatureBuilder.add_texture(D3D12_SHADER_VISIBILITY_PIXEL);
	rootSignatureBuilder.sampler(
		D3D12_SHADER_VISIBILITY_PIXEL,
		0,
		D3D12_FILTER_ANISOTROPIC
	);

	InputLayoutBuilder inputLayoutBuilder;
	inputLayoutBuilder.add_element("POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT);
	inputLayoutBuilder.add_element("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT);

	ShaderBuilder shaderBuilder;
	shaderBuilder.initialize(
		"EngineResources/HLSL/Sprite/Sprite.VS.hlsl",
		"EngineResources/HLSL/Sprite/Sprite.PS.hlsl"
	);

	std::unique_ptr<PSOBuilder> psoBuilder = std::make_unique<PSOBuilder>();
	psoBuilder->blendstate(BlendMode::Normal);
	psoBuilder->inputlayout(inputLayoutBuilder.build());
	psoBuilder->rasterizerstate(D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_NONE);
	psoBuilder->rootsignature(rootSignatureBuilder.build());
	psoBuilder->shaders(shaderBuilder);
	psoBuilder->primitivetopologytype();
	psoBuilder->rendertarget();

	pipelineState = std::make_unique<PipelineState>();
	pipelineState->initialize(psoBuilder->get_rootsignature(), psoBuilder->build());

}
