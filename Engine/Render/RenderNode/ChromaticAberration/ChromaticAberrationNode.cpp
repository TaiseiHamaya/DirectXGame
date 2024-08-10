#include "ChromaticAberrationNode.h"

#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/DirectX/PipelineState/PipelineState.h"
#include "Engine/DirectX/PipelineState/PSOBuilder/PSOBuilder.h"

#ifdef _DEBUG
#include "externals/imgui/imgui.h"
#include "Engine/WinApp.h"
#endif // _DEBUG

ChromaticAberrationNode::ChromaticAberrationNode() = default;

ChromaticAberrationNode::~ChromaticAberrationNode() noexcept = default;

void ChromaticAberrationNode::initialize() {
	create_pipeline_state();
	primitiveTopology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	*aberrationLevel.get_data() = CVector2::ZERO;
}

void ChromaticAberrationNode::draw() {
	auto&& command = DirectXCommand::GetCommandList();
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

	ShaderBuilder shaderManager;
	shaderManager.initialize(
		"Engine/HLSL/FullscreenShader.hlsl",
		"Engine/Render/RenderNode/ChromaticAberration/ChromaticAberration.PS.hlsl"
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
void ChromaticAberrationNode::debug_gui() {
	ImGui::DragFloat("AberrationLevelX", &aberrationLevel.get_data()->x, 0.1f / WinApp::GetClientWidth(), -0.5f, 0.5f, "%.4f");
	ImGui::DragFloat("AberrationLevelY", &aberrationLevel.get_data()->y, 0.1f / WinApp::GetClientHight(), -0.5f, 0.5f, "%.4f");
}
#endif // _DEBUG
