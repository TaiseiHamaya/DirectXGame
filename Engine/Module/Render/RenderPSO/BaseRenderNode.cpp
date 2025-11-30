#include "BaseRenderNode.h"

#include <d3d12.h>

#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"
#include "Engine/GraphicsAPI/DirectX/DxPipelineState/DxPipelineState.h"

BaseRenderNode::BaseRenderNode() = default;
BaseRenderNode::~BaseRenderNode() noexcept = default;

void BaseRenderNode::stack_pso_command() {
	auto&& commandList = DxCommand::GetCommandList();
	// PSOの設定
	pipelineState->set_graphics_pipeline_state();
	// RootSignatureの設定
	pipelineState->set_root_signature();
	// 三角ポリゴン描画設定
	commandList->IASetPrimitiveTopology(primitiveTopology);
}
