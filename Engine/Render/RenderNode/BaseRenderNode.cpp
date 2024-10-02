#include "BaseRenderNode.h"

#include <d3d12.h>

#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/DirectX/PipelineState/PipelineState.h"
#include "Engine/Render/RenderTargetGroup/BaseRenderTargetGroup.h"

BaseRenderNode::BaseRenderNode() = default;

BaseRenderNode::~BaseRenderNode() noexcept = default;

void BaseRenderNode::finalize() {
	if (renderTarget.use_count() == 1) {
		renderTarget->finalize();
	}
}

void BaseRenderNode::begin() {
	auto&& commandList = DirectXCommand::GetCommandList();

	renderTarget->begin(config);
	pipelineState->set_graphics_pipeline_state();
	pipelineState->set_root_signature();
	// 三角ポリゴン描画設定
	commandList->IASetPrimitiveTopology(primitiveTopology);
}

void BaseRenderNode::end() {
	renderTarget->end(config);
}

const std::shared_ptr<BaseRenderTargetGroup>& BaseRenderNode::get_render_target_group() const {
	return renderTarget;
}

void BaseRenderNode::set_rt_config(const eps::bitflag<BaseRenderTargetGroup::RTGConfing>& config_) {
	config = config_;
}
