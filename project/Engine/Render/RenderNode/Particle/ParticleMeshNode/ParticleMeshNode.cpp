#include "ParticleMeshNode.h"

#include "Engine/DirectX/DirectXSwapChain/DirectXSwapChain.h"
#include "Engine/DirectX/PipelineState/PipelineState.h"
#include "Engine/DirectX/PipelineState/PSOBuilder/PSOBuilder.h"

ParticleMeshNode::ParticleMeshNode() = default;

ParticleMeshNode::~ParticleMeshNode() noexcept = default;

void ParticleMeshNode::initialize() {
	depthStencil = DirectXSwapChain::GetDepthStencil();
	create_pipeline_state();
	pipelineState->set_name("Object3DNode");
	primitiveTopology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

void ParticleMeshNode::create_pipeline_state() {
	RootSignatureBuilder rootSignatureBuilder;
	rootSignatureBuilder.add_structured(D3D12_SHADER_VISIBILITY_VERTEX, 0, 1); // 0 :  transform
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_VERTEX, 0); // 1 : camera
	rootSignatureBuilder.add_texture(D3D12_SHADER_VISIBILITY_PIXEL, 0); // 2 : texture
	rootSignatureBuilder.add_structured(D3D12_SHADER_VISIBILITY_PIXEL, 1, 1); // 3 : material
	rootSignatureBuilder.sampler( // sampler
		D3D12_SHADER_VISIBILITY_PIXEL,
		0,
		D3D12_FILTER_ANISOTROPIC
	);

	InputLayoutBuilder inputLayoutBuilder;
	inputLayoutBuilder.add_element("POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT);
	inputLayoutBuilder.add_element("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT);

	ShaderBuilder shaderManager;
	shaderManager.initialize(
		"EngineResources/HLSL/Particle/ParticleMesh/ParticleMesh.VS.hlsl",
		"EngineResources/HLSL/Particle/ParticleMesh/ParticleMesh.PS.hlsl"
	);

	std::unique_ptr<PSOBuilder> psoBuilder = std::make_unique<PSOBuilder>();
	psoBuilder->blendstate();
	psoBuilder->depthstencilstate(*depthStencil);
	psoBuilder->inputlayout(inputLayoutBuilder.build());
	psoBuilder->rasterizerstate();
	psoBuilder->rootsignature(rootSignatureBuilder.build());
	psoBuilder->shaders(shaderManager);
	psoBuilder->primitivetopologytype();
	psoBuilder->rendertarget();

	pipelineState = std::make_unique<PipelineState>();
	pipelineState->initialize(psoBuilder->get_rootsignature(), psoBuilder->build());

}
