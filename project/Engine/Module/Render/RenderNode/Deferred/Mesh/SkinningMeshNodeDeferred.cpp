#include "SkinningMeshNodeDeferred.h"

#include "../DeferredAdaptor.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/DepthStencil/DepthStencil.h"
#include "Engine/GraphicsAPI/DirectX/PipelineState/PipelineState.h"
#include "Engine/GraphicsAPI/DirectX/PipelineState/PSOBuilder/PSOBuilder.h"

SkinningMeshNodeDeferred::SkinningMeshNodeDeferred() = default;

SkinningMeshNodeDeferred::~SkinningMeshNodeDeferred() noexcept = default;

void SkinningMeshNodeDeferred::initialize() {
	depthStencil = DepthStencilValue::depthStencil;
	create_pipeline_state();
	pipelineState->set_name("SkinningMeshNodeDeferred");
	primitiveTopology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

void SkinningMeshNodeDeferred::create_pipeline_state() {
	RootSignatureBuilder rootSignatureBuilder;
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_VERTEX, 0); // 0 :  transform
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_VERTEX, 1); // 1 : camera
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_PIXEL, 0); // 2 : material
	rootSignatureBuilder.add_texture(D3D12_SHADER_VISIBILITY_PIXEL); // 3 : texture
	rootSignatureBuilder.add_structured(D3D12_SHADER_VISIBILITY_VERTEX, 1); // 4 : bone
	rootSignatureBuilder.sampler( // sampler
		D3D12_SHADER_VISIBILITY_PIXEL,
		0,
		D3D12_FILTER_ANISOTROPIC
	);

	InputLayoutBuilder inputLayoutBuilder;
	inputLayoutBuilder.add_element("POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	inputLayoutBuilder.add_element("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0);
	inputLayoutBuilder.add_element("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0);

	inputLayoutBuilder.add_element("WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1);
	inputLayoutBuilder.add_element("INDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 1);

	ShaderBuilder shaderBuilder;
	shaderBuilder.initialize(
		"EngineResources/HLSL/Deferred/Mesh/SkinningMesh.VS.hlsl",
		"EngineResources/HLSL/Deferred/Deferred.PS.hlsl"
	);

	std::unique_ptr<PSOBuilder> psoBuilder = std::make_unique<PSOBuilder>();
	psoBuilder->depthstencilstate(*depthStencil);
	psoBuilder->inputlayout(inputLayoutBuilder.build());
	psoBuilder->rasterizerstate();
	psoBuilder->rootsignature(rootSignatureBuilder.build());
	psoBuilder->shaders(shaderBuilder);
	psoBuilder->primitivetopologytype();
	psoBuilder->blendstate_only_write();
	for (uint32_t i = 0; i < DeferredAdaptor::NUM_GBUFFER; ++i) {
		psoBuilder->rendertarget(DeferredAdaptor::DXGI_FORMAT_LIST[i]);
	}

	pipelineState = std::make_unique<PipelineState>();
	pipelineState->initialize(psoBuilder->get_rootsignature(), psoBuilder->build());
}
