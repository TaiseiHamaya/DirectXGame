#include "GrayscaleNode.h"

#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/DirectX/DirectXResourceObject/VertexBuffer/VertexBuffer.h"
#include "Engine/DirectX/PipelineState/PipelineState.h"
#include "Engine/DirectX/PipelineState/PSOBuilder/PSOBuilder.h"

GrayscaleNode::GrayscaleNode() = default;

GrayscaleNode::~GrayscaleNode() = default;

void GrayscaleNode::initialize() {
	create_pipline_state();
	primitiveTopology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	create_vertex();
}

void GrayscaleNode::draw() {
	auto&& command = DirectXCommand::GetCommandList();
	command->IASetVertexBuffers(0, 1, vertex->get_p_vbv());
	command->SetGraphicsRootDescriptorTable(0, textureGPUHandle);
	command->DrawInstanced(6, 1, 0, 0);
}

void GrayscaleNode::set_texture_resource(const D3D12_GPU_DESCRIPTOR_HANDLE& textureGPUHandle_) {
	textureGPUHandle = textureGPUHandle_;
}

void GrayscaleNode::create_pipline_state() {
	RootSignatureBuilder rootSignatureBuilder;
	rootSignatureBuilder.descriptor_range();
	rootSignatureBuilder.add_texture(D3D12_SHADER_VISIBILITY_PIXEL);
	rootSignatureBuilder.sampler(
		D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_COMPARISON_FUNC_NEVER,
		D3D12_SHADER_VISIBILITY_PIXEL,
		0
	);

	InputLayoutBuillder inputLayoutBuillder;
	inputLayoutBuillder.add_cbv("POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT);
	inputLayoutBuillder.add_cbv("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT);
	inputLayoutBuillder.add_cbv("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT);

	ShaderBuilder shaderManager;
	shaderManager.initialize(
		"Engine/Render/RenderNode/Grayscale/Grayscale.VS.hlsl",
		"Engine/Render/RenderNode/Grayscale/Grayscale.PS.hlsl"
	);

	std::unique_ptr<PSOBuilder> psoBuilder = std::make_unique<PSOBuilder>();
	psoBuilder->blendstate();
	psoBuilder->inputlayout(inputLayoutBuillder.build());
	psoBuilder->rasterizerstate();
	psoBuilder->rootsignature(rootSignatureBuilder.build());
	psoBuilder->shaders(shaderManager);
	psoBuilder->primitivetopologytype();

	pipelineState = std::make_unique<PipelineState>();
	pipelineState->initialize(psoBuilder->get_rootsignature(), psoBuilder->build());
}

void GrayscaleNode::create_vertex() {
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
