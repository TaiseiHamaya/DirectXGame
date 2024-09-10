#include "BaseRenderNode.h"

#include <d3d12.h>

#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/DirectX/PipelineState/PipelineState.h"
#include "Engine/Render/RenderTargetGroup/BaseRenderTargetGroup.h"

BaseRenderNode::BaseRenderNode() = default;

BaseRenderNode::~BaseRenderNode() noexcept = default;

void BaseRenderNode::finalize() {
	renderTarget->finalize();
}

void BaseRenderNode::begin() {
	auto&& commandList = DirectXCommand::GetCommandList();

	renderTarget->begin();
	pipelineState->set_graphics_pipeline_state();
	pipelineState->set_root_signature();
	// 三角ポリゴン描画設定
	commandList->IASetPrimitiveTopology(primitiveTopology);
}

void BaseRenderNode::end() {
	renderTarget->end();
}

const std::shared_ptr<BaseRenderTargetGroup>& BaseRenderNode::get_render_target_group() const {
	return renderTarget;
}
