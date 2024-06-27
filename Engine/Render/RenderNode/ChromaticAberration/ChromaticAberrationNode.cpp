#include "ChromaticAberrationNode.h"

#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/DirectX/DirectXResourceObject/VertexBuffer/VertexBuffer.h"
#include "Engine/DirectX/PipelineState/PipelineState.h"
#include "Engine/DirectX/PipelineState/PSOBuilder/PSOBuilder.h"

#ifdef _DEBUG
#include "externals/imgui/imgui.h"
#include "Engine/WinApp.h"
#endif // _DEBUG

ChromaticAberrationNode::ChromaticAberrationNode() = default;

ChromaticAberrationNode::~ChromaticAberrationNode() noexcept = default;

void ChromaticAberrationNode::initialize() {
	create_pipline_state();
	primitiveTopology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	create_vertex();
	*aberrationLevel.get_data() = CVector2::ZERO;
}

void ChromaticAberrationNode::draw() {
	auto&& command = DirectXCommand::GetCommandList();
	command->IASetVertexBuffers(0, 1, vertex->get_p_vbv());
	command->SetGraphicsRootConstantBufferView(0, aberrationLevel.get_resource()->GetGPUVirtualAddress());
	command->SetGraphicsRootDescriptorTable(1, textureGPUHandle);
	command->DrawInstanced(6, 1, 0, 0);
}

void ChromaticAberrationNode::set_texture_resource(const D3D12_GPU_DESCRIPTOR_HANDLE& textureGPUHandle_) {
	textureGPUHandle = textureGPUHandle_;
}

void ChromaticAberrationNode::create_pipline_state() {
	RootSignatureBuilder rootSignatureBuilder;
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_PIXEL, 0);
	rootSignatureBuilder.add_texture(D3D12_SHADER_VISIBILITY_PIXEL, 0, 1);
	rootSignatureBuilder.sampler(
		D3D12_SHADER_VISIBILITY_PIXEL,
		0
	);

	InputLayoutBuillder inputLayoutBuillder;
	inputLayoutBuillder.add_cbv("POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT);
	inputLayoutBuillder.add_cbv("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT);
	inputLayoutBuillder.add_cbv("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT);

	ShaderBuilder shaderManager;
	shaderManager.initialize(
		"Engine/Render/RenderNode/ChromaticAberration/ChromaticAberration.VS.hlsl",
		"Engine/Render/RenderNode/ChromaticAberration/ChromaticAberration.PS.hlsl"
	);

	std::unique_ptr<PSOBuilder> psoBuilder = std::make_unique<PSOBuilder>();
	psoBuilder->blendstate();
	psoBuilder->inputlayout(inputLayoutBuillder.build());
	psoBuilder->rasterizerstate();
	psoBuilder->rootsignature(rootSignatureBuilder.build());
	psoBuilder->shaders(shaderManager);
	psoBuilder->primitivetopologytype();
	psoBuilder->rendertarget();

	pipelineState = std::make_unique<PipelineState>();
	pipelineState->initialize(psoBuilder->get_rootsignature(), psoBuilder->build());

}

void ChromaticAberrationNode::create_vertex() {
	std::vector<VertexData> vertexData(6);
	vertexData[0].vertex = VertexData::Vector4{ {-1, 1, 0}, 1 };
	vertexData[0].texcoord = CVector2::ZERO;

	vertexData[1].vertex = VertexData::Vector4{ { 1, -1, 0}, 1 };
	vertexData[1].texcoord = CVector2::BASIS;

	vertexData[2].vertex = VertexData::Vector4{ {-1, -1, 0}, 1 };
	vertexData[2].texcoord = CVector2::BASIS_Y;

	vertexData[3] = vertexData[0];

	vertexData[4].vertex = VertexData::Vector4{ { 1, 1, 0}, 1 };
	vertexData[4].texcoord = CVector2::BASIS_X;

	vertexData[5] = vertexData[1];
	vertex = std::make_unique<VertexBuffer>(vertexData);
}

void ChromaticAberrationNode::debug_gui() {
	ImGui::DragFloat("AberrationLevelX", &aberrationLevel.get_data()->x, 0.1f / WinApp::GetClientWidth(), -0.5f, 0.5f, "%.4f");
	ImGui::DragFloat("AberrationLevelY", &aberrationLevel.get_data()->y, 0.1f / WinApp::GetClientHight(), -0.5f, 0.5f, "%.4f");
}
