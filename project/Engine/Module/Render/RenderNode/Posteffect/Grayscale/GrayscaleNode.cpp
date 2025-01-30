#include "GrayscaleNode.h"

#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"
#include "Engine/GraphicsAPI/DirectX/PipelineState/PipelineState.h"
#include "Engine/GraphicsAPI/DirectX/PipelineState/PSOBuilder/PSOBuilder.h"

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

GrayscaleNode::GrayscaleNode() = default;

GrayscaleNode::~GrayscaleNode() = default;

void GrayscaleNode::initialize() {
	create_pipeline_state();
	primitiveTopology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	*isGray.get_data() = false;
}

void GrayscaleNode::draw() {
	auto&& command = DxCommand::GetCommandList();
	command->SetGraphicsRootDescriptorTable(1, textureGPUHandle);
	command->SetGraphicsRootConstantBufferView(0, isGray.get_resource()->GetGPUVirtualAddress());
	command->DrawInstanced(3, 1, 0, 0);
}

void GrayscaleNode::set_texture_resource(const D3D12_GPU_DESCRIPTOR_HANDLE& textureGPUHandle_) {
	textureGPUHandle = textureGPUHandle_;
}

void GrayscaleNode::create_pipeline_state() {
	RootSignatureBuilder rootSignatureBuilder;
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_PIXEL, 0);
	rootSignatureBuilder.add_texture(D3D12_SHADER_VISIBILITY_PIXEL);
	rootSignatureBuilder.sampler(
		D3D12_SHADER_VISIBILITY_PIXEL,
		0
	);

	ShaderBuilder shaderManager;
	shaderManager.initialize(
		"EngineResources/HLSL/FullscreenShader.hlsl",
		"EngineResources/HLSL/Posteffect/Grayscale/Grayscale.PS.hlsl"
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
void GrayscaleNode::debug_gui() {
	ImGui::Checkbox("IsGray", reinterpret_cast<bool*>(isGray.get_data()));
}
#endif // _DEBUG
