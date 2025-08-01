#include "PostEffectNode.h"

#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"

void PostEffectNode::stack_command() {
	auto&& commandList = DxCommand::GetCommandList();
	// ViewPortの設定
	commandList->RSSetViewports(1, &viewport);
	// シザー矩形の設定
	commandList->RSSetScissorRects(1, &rect);

	outRenderTargetGroup->begin_write(true);

	postEffectPSO->stack_pso_command();

	postEffectPSO->execute_effect_command();
}
