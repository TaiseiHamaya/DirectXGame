#include "LuminanceExtractionNode.h"

#include "Engine/Module/Render/RenderTargetGroup/SingleRenderTarget.h"
#include "Engine/Rendering/DirectX/DirectXResourceObject/OffscreenRender/OffscreenRender.h"
#include "Engine/Rendering/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/Rendering/DirectX/PipelineState/PipelineState.h"
#include "Engine/Rendering/DirectX/PipelineState/PSOBuilder/PSOBuilder.h"

void LuminanceExtractionNode::initialize() {
	create_pipeline_state();
	pipelineState->set_name("LuminanceExtractionNode");
	primitiveTopology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	luminanceExtractionInfo.get_data()->intensity = 0.3f;
}

void LuminanceExtractionNode::draw() const {
	auto&& command = DirectXCommand::GetCommandList();
	command->SetGraphicsRootConstantBufferView(0, luminanceExtractionInfo.get_resource()->GetGPUVirtualAddress());
	command->SetGraphicsRootDescriptorTable(1, textureGPUHandle);
	command->DrawInstanced(3, 1, 0, 0);

}

void LuminanceExtractionNode::set_texture_resource(const D3D12_GPU_DESCRIPTOR_HANDLE& textureGPUHandle_) {
	textureGPUHandle = textureGPUHandle_;
}

void LuminanceExtractionNode::set_render_target(const std::shared_ptr<SingleRenderTarget>& renderTarget_) {
	if (renderTarget_) {
		renderTarget = renderTarget_;
		resultSvtHandle = renderTarget_->offscreen_render().texture_gpu_handle();
	}
	else {
		auto temp = std::make_shared<SingleRenderTarget>();
		renderTarget = temp;
		renderTarget->initialize();
		temp->offscreen_render().set_claer_color(Color4{ 0.0f,0.0f,0.0f,1.0f });
		resultSvtHandle = temp->offscreen_render().texture_gpu_handle();
	}
}

void LuminanceExtractionNode::create_pipeline_state() {
	RootSignatureBuilder rootSignatureBuilder;
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_PIXEL, 0);
	rootSignatureBuilder.add_texture(D3D12_SHADER_VISIBILITY_PIXEL);
	rootSignatureBuilder.sampler(
		D3D12_SHADER_VISIBILITY_PIXEL,
		0,
		D3D12_FILTER_MIN_MAG_MIP_LINEAR
	);

	ShaderBuilder shaderManager;
	shaderManager.initialize(
		"EngineResources/HLSL/FullscreenShader.hlsl",
		"Resources/HLSL/PostEffetct/LuminanceExtraction.PS.hlsl"
	);

	std::unique_ptr<PSOBuilder> psoBuilder = std::make_unique<PSOBuilder>();
	psoBuilder->blendstate();
	psoBuilder->rasterizerstate();
	psoBuilder->rootsignature(rootSignatureBuilder.build());
	psoBuilder->shaders(shaderManager);
	psoBuilder->primitivetopologytype();
	psoBuilder->rendertarget();

	pipelineState = std::make_unique<PipelineState>();
	pipelineState->initialize(psoBuilder->get_rootsignature(), psoBuilder->build());
}

#ifdef _DEBUG
#include <imgui.h>
void LuminanceExtractionNode::debug_gui() {
	ImGui::DragFloat("Intensity", &luminanceExtractionInfo.get_data()->intensity, 0.001f, 0.0f, 1.0f, "%.4f");
}
#endif // _DEBUG
