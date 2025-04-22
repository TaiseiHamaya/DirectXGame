#include "SkinningMeshNodeForward.h"

#include "Engine/GraphicsAPI/DirectX/DxResource/DepthStencil/DepthStencil.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/OffscreenRender/OffscreenRender.h"
#include "Engine/GraphicsAPI/DirectX/DxPipelineState/DxPipelineState.h"
#include "Engine/GraphicsAPI/DirectX/DxPipelineState/PSOBuilder/PSOBuilder.h"
#include "Engine/GraphicsAPI/RenderingSystemValues.h"
#include "Engine/Module/Render/RenderTargetGroup/SingleRenderTarget.h"

SkinningMeshNodeForward::SkinningMeshNodeForward() = default;
SkinningMeshNodeForward::~SkinningMeshNodeForward() noexcept = default;

void SkinningMeshNodeForward::initialize() {
	depthStencil = DepthStencilValue::depthStencil;
	create_pipeline_state();
	pipelineState->set_name("SkinningMeshNodeForward");
	primitiveTopology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

void SkinningMeshNodeForward::set_render_target(const std::shared_ptr<SingleRenderTarget>& renderTarget_) {
	if (renderTarget_) {
		renderTarget = renderTarget_;
		resultSvtHandle = renderTarget_->offscreen_render().texture_gpu_handle();
		renderTarget_->offscreen_render().set_clear_color(RenderingSystemValues::DEFAULT_CLEAR_COLOR);
	}
	else {
		auto temp = std::make_shared<SingleRenderTarget>();
		renderTarget = temp;
		renderTarget->initialize();
		resultSvtHandle = temp->offscreen_render().texture_gpu_handle();
		temp->offscreen_render().set_clear_color(RenderingSystemValues::DEFAULT_CLEAR_COLOR);
	}
}

void SkinningMeshNodeForward::create_pipeline_state() {
	RootSignatureBuilder rootSignatureBuilder;
	rootSignatureBuilder.add_structured(D3D12_SHADER_VISIBILITY_VERTEX, 0, 1, 0); // 0 : transform(S0T0, V)
	rootSignatureBuilder.add_structured(D3D12_SHADER_VISIBILITY_PIXEL, 0, 1, 0); // 1 : material(S0T0, P)
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_VERTEX, 0, 1); // 2 : camera proj(S1B0, V)
	rootSignatureBuilder.add_structured(D3D12_SHADER_VISIBILITY_VERTEX, 1, 1, 0); // 3 : bone(S0T1, V)
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_PIXEL, 0, 1); // 4 : camera position(S1B0, P)
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_PIXEL, 0, 2); // 5 : light(S2B0, P)
	rootSignatureBuilder.sampler( // sampler
		D3D12_SHADER_VISIBILITY_PIXEL,
		0, 0,
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
		"DirectXGame/EngineResources/HLSL/Forward/Mesh/SkinningMesh.VS.hlsl",
		"DirectXGame/EngineResources/HLSL/Forward/Forward.PS.hlsl"
	);

	std::unique_ptr<PSOBuilder> psoBuilder = std::make_unique<PSOBuilder>();
	psoBuilder->blendstate();
	psoBuilder->depthstencilstate(*depthStencil);
	psoBuilder->inputlayout(inputLayoutBuilder.build());
	psoBuilder->rasterizerstate();
	psoBuilder->rootsignature(rootSignatureBuilder.build());
	psoBuilder->shaders(shaderBuilder);
	psoBuilder->primitivetopologytype();
	psoBuilder->rendertarget();

	pipelineState = std::make_unique<DxPipelineState>();
	pipelineState->initialize(psoBuilder->get_rootsignature(), psoBuilder->build());
}
