#ifdef _DEBUG

#include "PrimitiveLineNode.h"

#include "Engine/GraphicsAPI/DirectX/DxResource/DepthStencil/DepthStencil.h"
#include "Engine/GraphicsAPI/DirectX/DxSwapChain/DxSwapChain.h"
#include "Engine/GraphicsAPI/DirectX/PipelineState/PipelineState.h"
#include "Engine/GraphicsAPI/DirectX/PipelineState/PSOBuilder/PSOBuilder.h"

PrimitiveLineNode::PrimitiveLineNode() = default;
PrimitiveLineNode::~PrimitiveLineNode() noexcept = default;

void PrimitiveLineNode::initialize() {
	depthStencil = DepthStencilValue::depthStencil;
	create_pipeline_state();
	pipelineState->set_name("PrimitiveLineNode");
	primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
	set_render_target_SC(DxSwapChain::GetRenderTarget());
	set_config(RenderNodeConfig::ContinueDrawAfter |
		RenderNodeConfig::ContinueDrawBefore |
		RenderNodeConfig::NoClearDepth
	);
}

void PrimitiveLineNode::create_pipeline_state() {
	RootSignatureBuilder rootSignatureBuilder;
	rootSignatureBuilder.add_structured(D3D12_SHADER_VISIBILITY_VERTEX, 0); // 0 :  transform
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_VERTEX, 0); // 1 : camera

	InputLayoutBuilder inputLayoutBuilder;
	inputLayoutBuilder.add_element("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT);

	ShaderBuilder shaderBuilder;
	shaderBuilder.initialize(
		"EngineResources/HLSL/Misc/PrimitiveGeometry/PrimitiveGeometry.VS.hlsl",
		"EngineResources/HLSL/Misc/PrimitiveGeometry/PrimitiveGeometry.PS.hlsl"
	);

	std::unique_ptr<PSOBuilder> psoBuilder = std::make_unique<PSOBuilder>();
	psoBuilder->blendstate();
	psoBuilder->rasterizerstate();
	psoBuilder->depth_state(depthStencil->get_resource()->GetDesc().Format, D3D12_DEPTH_WRITE_MASK_ZERO);
	psoBuilder->inputlayout(inputLayoutBuilder.build());
	psoBuilder->rootsignature(rootSignatureBuilder.build());
	psoBuilder->shaders(shaderBuilder);
	psoBuilder->primitivetopologytype(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE);
	psoBuilder->rendertarget();

	pipelineState = std::make_unique<PipelineState>();
	pipelineState->initialize(psoBuilder->get_rootsignature(), psoBuilder->build());
}

#endif // _DEBUG
