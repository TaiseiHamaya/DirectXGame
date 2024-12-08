#include "GaussianBlurNode.h"

#include "Engine/Module/Render/RenderTargetGroup/SingleRenderTarget.h"
#include "Engine/Rendering/DirectX/DirectXResourceObject/OffscreenRender/OffscreenRender.h"
#include "Engine/Rendering/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/Rendering/DirectX/PipelineState/PipelineState.h"
#include "Engine/Rendering/DirectX/PipelineState/PSOBuilder/PSOBuilder.h"

#include "Engine/Application/EngineSettings.h"

void GaussianBlurNode::initialize() {
	create_pipeline_state();
	pipelineState->set_name("GaussianBlur");
	primitiveTopology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	blurInfo.get_data()->dispersion = 0.4f;
	blurInfo.get_data()->length = 6.0f;
	blurInfo.get_data()->sampleCount = 8;
}

void GaussianBlurNode::draw() const {
	auto&& command = DirectXCommand::GetCommandList();
	command->SetGraphicsRootConstantBufferView(0, blurInfo.get_resource()->GetGPUVirtualAddress());
	command->SetGraphicsRootDescriptorTable(1, textureGPUHandle);
	command->DrawInstanced(3, 1, 0, 0);
}

void GaussianBlurNode::set_texture_resource(const D3D12_GPU_DESCRIPTOR_HANDLE& textureGPUHandle_) {
	textureGPUHandle = textureGPUHandle_;
}

void GaussianBlurNode::set_render_target(const std::shared_ptr<SingleRenderTarget>& renderTarget_) {
	if (renderTarget_) {
		renderTarget = renderTarget_;
		resultSvtHandle = renderTarget_->offscreen_render().texture_gpu_handle();
	}
	else {
		auto temp = std::make_shared<SingleRenderTarget>();
		renderTarget = temp;
		// 半分の大きさにさせる
		temp->initialize(EngineSettings::CLIENT_WIDTH / 2, EngineSettings::CLIENT_HEIGHT / 2);
		resultSvtHandle = temp->offscreen_render().texture_gpu_handle();
	}
}

void GaussianBlurNode::create_pipeline_state() {
	RootSignatureBuilder rootSignatureBuilder;
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_PIXEL, 0);
	rootSignatureBuilder.add_texture(D3D12_SHADER_VISIBILITY_PIXEL);
	rootSignatureBuilder.sampler(
		D3D12_SHADER_VISIBILITY_PIXEL,
		0,
		D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP
	);

	ShaderBuilder shaderManager;
	shaderManager.initialize(
		"EngineResources/HLSL/FullscreenShader.hlsl",
		"Resources/HLSL/PostEffetct/GaussianBlur.PS.hlsl"
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
void GaussianBlurNode::debug_gui() {
	ImGui::DragFloat("Weight", &blurInfo.get_data()->dispersion, 0.001f, 0.0f, 1.0f, "%.4f");
	ImGui::DragFloat("Length", &blurInfo.get_data()->length, 0.01f);
	static uint32_t min = 1;
	static uint32_t max = 16;
	ImGui::DragScalar("SampleCount", ImGuiDataType_U32, reinterpret_cast<int*>(&blurInfo.get_data()->sampleCount), 0.02f, &min, &max);
}
#endif // _DEBUG
