#include "BloomNode.h"

#include "Engine/Rendering/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/Rendering/DirectX/PipelineState/PipelineState.h"
#include "Engine/Rendering/DirectX/PipelineState/PSOBuilder/PSOBuilder.h"

void BloomNode::initialize() {
	create_pipeline_state();
	pipelineState->set_name("BloomNode");
	primitiveTopology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	bloomInfo.get_data()->weight = 1.0f;
}

void BloomNode::draw() const {
	auto&& command = DirectXCommand::GetCommandList();
	command->SetGraphicsRootConstantBufferView(0, bloomInfo.get_resource()->GetGPUVirtualAddress());
	command->SetGraphicsRootDescriptorTable(1, baseTexture);
	command->SetGraphicsRootDescriptorTable(2, blurTexture);
	command->DrawInstanced(3, 1, 0, 0);
}

void BloomNode::set_base_texture(const D3D12_GPU_DESCRIPTOR_HANDLE& textureGPUHandle_) {
	baseTexture = textureGPUHandle_;
}

void BloomNode::set_blur_texture(const D3D12_GPU_DESCRIPTOR_HANDLE& textureGPUHandle_) {
	blurTexture = textureGPUHandle_;
}

void BloomNode::create_pipeline_state() {
	RootSignatureBuilder rootSignatureBuilder;
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_PIXEL, 0);
	rootSignatureBuilder.add_texture(D3D12_SHADER_VISIBILITY_PIXEL, 0);
	rootSignatureBuilder.add_texture(D3D12_SHADER_VISIBILITY_PIXEL, 1);
	rootSignatureBuilder.sampler(
		D3D12_SHADER_VISIBILITY_PIXEL,
		0,
		D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP
	);

	ShaderBuilder shaderManager;
	shaderManager.initialize(
		"EngineResources/HLSL/FullscreenShader.hlsl",
		"Resources/HLSL/PostEffetct/Bloom.PS.hlsl"
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
void BloomNode::debug_gui() {
	ImGui::DragFloat("Weight", &bloomInfo.get_data()->weight, 0.001f, 0.0f, 1.0f, "%.4f");
}
#endif // _DEBUG
