#ifdef DEBUG_FEATURES_ENABLE

#include "PrimitiveLineNode.h"

#include "Engine/GraphicsAPI/DirectX/DxPipelineState/DxPipelineState.h"
#include "Engine/GraphicsAPI/DirectX/DxPipelineState/PSOBuilder/PSOBuilder.h"
#include "Engine/GraphicsAPI/RenderingSystemValues.h"

PrimitiveLineNode::PrimitiveLineNode() = default;
PrimitiveLineNode::~PrimitiveLineNode() noexcept = default;

void PrimitiveLineNode::initialize() {
	depthStencil = RenderingSystemValues::GetDepthStencilTexture();
	create_pipeline_state();
	pipelineState->set_name("PrimitiveLineNode");
	primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
	set_render_target_SC();
	set_config(RenderNodeConfig::NoClearRenderTarget | RenderNodeConfig::NoClearDepth);
}

void PrimitiveLineNode::create_pipeline_state() {
	RootSignatureBuilder rootSignatureBuilder;
	rootSignatureBuilder.add_structured(D3D12_SHADER_VISIBILITY_VERTEX, 0); // 0 :  transform
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_VERTEX, 0); // 1 : camera

	InputLayoutBuilder inputLayoutBuilder;
	inputLayoutBuilder.add_element("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT);

	std::unique_ptr<PSOBuilder> psoBuilder = std::make_unique<PSOBuilder>();
	psoBuilder->blendstate();
	psoBuilder->rasterizerstate();
	psoBuilder->depth_state(depthStencil->get_as_dsv()->get_format(), D3D12_DEPTH_WRITE_MASK_ZERO);
	psoBuilder->inputlayout(inputLayoutBuilder.build());
	psoBuilder->rootsignature(rootSignatureBuilder.build());
	psoBuilder->shaders(ShaderType::Vertex, "PrimitiveGeometry.VS.hlsl");
	psoBuilder->shaders(ShaderType::Pixel, "PrimitiveGeometry.PS.hlsl");
	psoBuilder->primitivetopologytype(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE);
	psoBuilder->rendertarget();

	pipelineState = std::make_unique<DxPipelineState>();
	pipelineState->initialize(psoBuilder->get_rootsignature(), psoBuilder->build());
}

#endif // _DEBUG
