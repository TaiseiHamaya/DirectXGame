#include "ChromaticAberrationNode.h"

#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"
#include "Engine/GraphicsAPI/DirectX/DxPipelineState/DxPipelineState.h"
#include "Engine/GraphicsAPI/DirectX/DxPipelineState/PSOBuilder/PSOBuilder.h"

#ifdef DEBUG_FEATURES_ENABLE
#include <imgui.h>
#include "Engine/Application/EngineSettings.h"
#endif // _DEBUG

ChromaticAberrationNode::ChromaticAberrationNode() = default;

ChromaticAberrationNode::~ChromaticAberrationNode() noexcept = default;

void ChromaticAberrationNode::initialize() {
	create_pipeline_state();
	primitiveTopology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	*aberrationLevel.get_data() = CVector2::ZERO;
}

void ChromaticAberrationNode::draw() {
	auto&& command = DxCommand::GetCommandList();
	command->SetGraphicsRootConstantBufferView(0, aberrationLevel.get_resource()->GetGPUVirtualAddress());
	command->SetGraphicsRootDescriptorTable(1, textureGPUHandle);
	command->DrawInstanced(3, 1, 0, 0);
}

void ChromaticAberrationNode::set_texture_resource(const D3D12_GPU_DESCRIPTOR_HANDLE& textureGPUHandle_) {
	textureGPUHandle = textureGPUHandle_;
}

void ChromaticAberrationNode::create_pipeline_state() {
	RootSignatureBuilder rootSignatureBuilder;
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_PIXEL, 0);
	rootSignatureBuilder.add_texture(D3D12_SHADER_VISIBILITY_PIXEL, 0, 1);
	rootSignatureBuilder.sampler(
		D3D12_SHADER_VISIBILITY_PIXEL,
		0
	);

	std::unique_ptr<PSOBuilder> psoBuilder = std::make_unique<PSOBuilder>();
	psoBuilder->blendstate();
	psoBuilder->rasterizerstate();
	psoBuilder->rootsignature(rootSignatureBuilder.build());
	psoBuilder->shaders(ShaderType::Vertex, "FullscreenShader.VS.hlsl");
	psoBuilder->shaders(ShaderType::Pixel, "ChromaticAberration.PS.hlsl");
	psoBuilder->primitivetopologytype();
	psoBuilder->rendertarget();

	pipelineState = std::make_unique<DxPipelineState>();
	pipelineState->initialize(psoBuilder->get_rootsignature(), psoBuilder->build());

}

#ifdef DEBUG_FEATURES_ENABLE
void ChromaticAberrationNode::debug_gui() {
	ImGui::DragFloat("AberrationLevelX", &aberrationLevel.get_data()->x, 0.1f / EngineSettings::CLIENT_WIDTH, -0.5f, 0.5f, "%.4f");
	ImGui::DragFloat("AberrationLevelY", &aberrationLevel.get_data()->y, 0.1f / EngineSettings::CLIENT_HEIGHT, -0.5f, 0.5f, "%.4f");
}
#endif // _DEBUG
