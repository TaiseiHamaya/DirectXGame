#include "FontRenderingNode.h"

#include "Engine/Assets/Shader/ShaderLibrary.h"
#include "Engine/GraphicsAPI/DirectX/DxPipelineState/DxPipelineState.h"
#include "Engine/GraphicsAPI/DirectX/DxPipelineState/PSOBuilder/PSOBuilder.h"
#include "Engine/GraphicsAPI/RenderingSystemValues.h"

FontRenderingNode::FontRenderingNode() = default;
FontRenderingNode::~FontRenderingNode() noexcept = default;

void FontRenderingNode::BeginLoadShader() {
	ShaderLibrary::RegisterLoadQue("[[szg]]/Forward/Font/MsdfFond.VS.hlsl");
	ShaderLibrary::RegisterLoadQue("[[szg]]/Forward/Font/MsdfFond.PS.hlsl");
}

void FontRenderingNode::initialize() {
	initialize(BlendMode::Alpha);
}

void FontRenderingNode::initialize(BlendMode blendMode) {
	create_pipeline_state(static_cast<PsoBlendMode>(blendMode));
	pipelineState->set_name("Rect3dNode");
	primitiveTopology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

void FontRenderingNode::create_pipeline_state(PsoBlendMode blendMode) {
	RootSignatureBuilder rootSignatureBuilder;
	rootSignatureBuilder.add_structured(D3D12_SHADER_VISIBILITY_VERTEX, 0, 1, 2); // gCharBufferVS
	rootSignatureBuilder.add_structured(D3D12_SHADER_VISIBILITY_ALL, 1, 1, 2); // gStringBuffer
	rootSignatureBuilder.add_structured(D3D12_SHADER_VISIBILITY_PIXEL, 0, 1, 0); // 2 : material(S0T0, P)
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_VERTEX, 0); // 3 : camera vs(S1B0, V)
	rootSignatureBuilder.sampler( // sampler
		D3D12_SHADER_VISIBILITY_PIXEL,
		0, 0,
		D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP
	);

	std::unique_ptr<PSOBuilder> psoBuilder = std::make_unique<PSOBuilder>();
	psoBuilder->depth_state(RenderingSystemValues::GetDepthStencilTexture()->get_as_dsv()->get_format(), D3D12_DEPTH_WRITE_MASK_ZERO);
	psoBuilder->rasterizerstate();
	psoBuilder->rootsignature(rootSignatureBuilder.build());
	psoBuilder->shaders(ShaderType::Vertex, "MsdfFond.VS.hlsl");
	psoBuilder->shaders(ShaderType::Pixel, "MsdfFond.PS.hlsl");
	psoBuilder->primitivetopologytype();
	psoBuilder->blendstate(blendMode);
	psoBuilder->rendertarget();

	pipelineState = std::make_unique<DxPipelineState>();
	pipelineState->initialize(psoBuilder->get_rootsignature(), psoBuilder->build());
}
