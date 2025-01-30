#include "BaseRenderNode.h"

#include <d3d12.h>

#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/DepthStencil/DepthStencil.h"
#include "Engine/GraphicsAPI/DirectX/PipelineState/PipelineState.h"
#include "Engine/Module/Render/RenderTargetGroup/BaseRenderTargetGroup.h"

BaseRenderNode::BaseRenderNode() = default;

BaseRenderNode::~BaseRenderNode() noexcept = default;

void BaseRenderNode::begin() {
	auto&& commandList = DxCommand::GetCommandList();
	// RenderTargetの設定
	renderTarget->begin(config, depthStencil);
	// DepthのStateを変える
	if (depthStencil && !(config & RenderNodeConfig::NoChangeDepthStateBegin)) {
		depthStencil->change_resource_state();
	}
	// DepthStencilを持っているならクリアする
	if (depthStencil && !(config & RenderNodeConfig::NoClearDepth)) {
		commandList->ClearDepthStencilView(
			depthStencil->get_dsv_cpu_handle(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr
		);
	}
	// PSOの設定
	pipelineState->set_graphics_pipeline_state();
	// RootSignatureの設定
	pipelineState->set_root_signature();
	// 三角ポリゴン描画設定
	commandList->IASetPrimitiveTopology(primitiveTopology);
}

void BaseRenderNode::end() {
	renderTarget->end(config);
	if (depthStencil && !(config & RenderNodeConfig::NoChangeDepthStateEnd)) {
		depthStencil->change_resource_state();
	}
}

const std::shared_ptr<BaseRenderTargetGroup>& BaseRenderNode::get_render_target_group() const {
	return renderTarget;
}

void BaseRenderNode::set_config(const eps::bitflag<RenderNodeConfig>& config_) {
	config = config_;
}
