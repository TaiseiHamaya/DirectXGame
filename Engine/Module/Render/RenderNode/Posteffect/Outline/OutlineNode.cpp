#include "OutlineNode.h"

#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"
#include "Engine/GraphicsAPI/DirectX/DxPipelineState/DxPipelineState.h"
#include "Engine/GraphicsAPI/DirectX/DxPipelineState/PSOBuilder/PSOBuilder.h"

OutlineNode::OutlineNode() = default;

OutlineNode::~OutlineNode() = default;

void OutlineNode::initialize() {
	create_pipeline_state();
	primitiveTopology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

void OutlineNode::draw() {
	auto&& command = DxCommand::GetCommandList();
	command->SetGraphicsRootDescriptorTable(0, textureGPUHandle);
	command->SetGraphicsRootDescriptorTable(1, depthGPUHandle);
	command->DrawInstanced(3, 1, 0, 0);
}

void OutlineNode::set_texture_resource(const D3D12_GPU_DESCRIPTOR_HANDLE& textureGPUHandle_) {
	textureGPUHandle = textureGPUHandle_;
}

void OutlineNode::set_depth_resource(const D3D12_GPU_DESCRIPTOR_HANDLE& depthGPUHandle_) {
	depthGPUHandle = depthGPUHandle_;
}

void OutlineNode::create_pipeline_state() {
	RootSignatureBuilder rootSignatureBuilder;
	rootSignatureBuilder.add_texture(D3D12_SHADER_VISIBILITY_PIXEL, 0, 1);
	rootSignatureBuilder.add_texture(D3D12_SHADER_VISIBILITY_PIXEL, 1, 1);
	rootSignatureBuilder.sampler(
		D3D12_SHADER_VISIBILITY_PIXEL,
		0
	);
	rootSignatureBuilder.sampler(
		D3D12_SHADER_VISIBILITY_PIXEL,
		1, 0,
		D3D12_FILTER_MIN_MAG_MIP_POINT,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP
	);

	ShaderBuilder shaderManager;
	shaderManager.initialize(
		"DirectXGame/EngineResources/HLSL/FullscreenShader.VS.hlsl",
		"DirectXGame/EngineResources/HLSL/Posteffect/Outline/Outline.PS.hlsl"
	);

	std::unique_ptr<PSOBuilder> psoBuilder = std::make_unique<PSOBuilder>();
	psoBuilder->blendstate();
	psoBuilder->rasterizerstate();
	psoBuilder->rootsignature(rootSignatureBuilder.build());
	psoBuilder->shaders(shaderManager);
	psoBuilder->primitivetopologytype();
	psoBuilder->rendertarget();

	pipelineState = std::make_unique<DxPipelineState>();
	pipelineState->initialize(psoBuilder->get_rootsignature(), psoBuilder->build());
}
