#include "BaseRenderNode.h"

#include <d3d12.h>

#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"
#include "Engine/GraphicsAPI/DirectX/DxPipelineState/DxPipelineState.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/TextureResource/DepthStencilTexture.h"
#include "Engine/Module/Render/RenderTargetGroup/BaseRenderTargetGroup.h"

BaseRenderNode::BaseRenderNode() = default;

BaseRenderNode::~BaseRenderNode() noexcept = default;

void BaseRenderNode::begin() {
	auto&& commandList = DxCommand::GetCommandList();
	// RenderTargetの設定
	renderTarget->begin_write(config, depthStencil);
	// DepthのStateを変える
	if (depthStencil) {
		depthStencil->start_write();
		if (!(config & RenderNodeConfig::NoClearDepth)) {
			depthStencil->get_as_dsv()->clear(1.0f, 0);
		}
	}
	// PSOの設定
	pipelineState->set_graphics_pipeline_state();
	// RootSignatureの設定
	pipelineState->set_root_signature();
	// 三角ポリゴン描画設定
	commandList->IASetPrimitiveTopology(primitiveTopology);
}

void BaseRenderNode::set_config(const eps::bitflag<RenderNodeConfig>& config_) {
	config = config_;
}
