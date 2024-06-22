#include "BaseRenderNode.h"

#include <d3d12.h>

#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/DirectX/DirectXResourceObject/DepthStencil/DepthStencil.h"
#include "Engine/DirectX/PipelineState/PipelineState.h"
#include "Engine/Math/Color.h"

BaseRenderNode::BaseRenderNode() = default;

BaseRenderNode::~BaseRenderNode() noexcept = default;

void BaseRenderNode::create_view_port(std::uint32_t width, std::uint32_t height) {
	viewPort = std::make_unique<D3D12_VIEWPORT>(
		0.0f, 0.0f,
		static_cast<FLOAT>(width),
		static_cast<FLOAT>(height),
		0.0f, 1.0f
	);
	scissorRect = std::make_unique<D3D12_RECT>(
		0, 0, width, height
	);
}

void BaseRenderNode::begin() {
	auto&& commandList = DirectXCommand::GetCommandList();

	pipelineState->set_graphics_pipeline_state();
	pipelineState->set_root_signature();
	if (depthStencil) {
		commandList->ClearDepthStencilView(
			depthStencil->get_cpu_handle(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr
		);
	}
	// ViewPortの設定
	commandList->RSSetViewports(1, viewPort.get());
	// シザー矩形の設定
	commandList->RSSetScissorRects(1, scissorRect.get());
	// 三角ポリゴン描画設定
	commandList->IASetPrimitiveTopology(primitiveTopology);
}

void BaseRenderNode::end() {
}
