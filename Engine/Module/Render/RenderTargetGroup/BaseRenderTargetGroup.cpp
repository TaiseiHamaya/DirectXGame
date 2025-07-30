#include "BaseRenderTargetGroup.h"

#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"

BaseRenderTargetGroup::BaseRenderTargetGroup() = default;

BaseRenderTargetGroup::~BaseRenderTargetGroup() noexcept = default;

void BaseRenderTargetGroup::begin_write(bool isClear, Reference<DepthStencilTexture> depthStencil) {
	auto&& commandList = DxCommand::GetCommandList();
	// レンダーターゲットを指定
	start_render_target(depthStencil);
	// レンダーターゲットのクリア
	if (isClear) {
		clear_render_target();
	}
}
