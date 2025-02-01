#include "StaticMeshNodeForward.h"

#include "Engine/GraphicsAPI/DirectX/DxResource/DepthStencil/DepthStencil.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/OffscreenRender/OffscreenRender.h"
#include "Engine/GraphicsAPI/DirectX/PipelineState/PipelineState.h"
#include "Engine/GraphicsAPI/DirectX/PipelineState/PSOBuilder/PSOBuilder.h"
#include "Engine/Module/Render/RenderTargetGroup/SingleRenderTarget.h"

#include "Engine/GraphicsAPI/RenderingSystemValues.h"

StaticMeshNodeForward::StaticMeshNodeForward() = default;
StaticMeshNodeForward::~StaticMeshNodeForward() noexcept = default;

void StaticMeshNodeForward::initialize() {
	depthStencil = DepthStencilValue::depthStencil;
	create_pipeline_state();
	pipelineState->set_name("StaticMeshNodeForward");
	primitiveTopology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

void StaticMeshNodeForward::set_render_target(const std::shared_ptr<SingleRenderTarget>& renderTarget_) {
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

void StaticMeshNodeForward::create_pipeline_state() {
	RootSignatureBuilder rootSignatureBuilder;
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_VERTEX, 0); // 0 :  transform
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_VERTEX, 1); // 1 : camera
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_PIXEL, 0); // 2 : material
	rootSignatureBuilder.add_texture(D3D12_SHADER_VISIBILITY_PIXEL); // 3 : texture
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_PIXEL, 1); // 4 : Camera
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_PIXEL, 2); // 5 : light
	rootSignatureBuilder.sampler( // sampler
		D3D12_SHADER_VISIBILITY_PIXEL,
		0,
		D3D12_FILTER_ANISOTROPIC
	);

	InputLayoutBuilder inputLayoutBuilder;
	inputLayoutBuilder.add_element("POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT);
	inputLayoutBuilder.add_element("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT);
	inputLayoutBuilder.add_element("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT);

	ShaderBuilder shaderBuilder;
	shaderBuilder.initialize(
		"EngineResources/HLSL/Forward/Mesh/StaticMesh.VS.hlsl",
		"EngineResources/HLSL/Forward/Forward.PS.hlsl"
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

	pipelineState = std::make_unique<PipelineState>();
	pipelineState->initialize(psoBuilder->get_rootsignature(), psoBuilder->build());
}
