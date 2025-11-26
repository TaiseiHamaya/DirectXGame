#include "WorldLayerRenderNode.h"

#include "../WorldRenderCollection.h"
#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"
#include "Engine/GraphicsAPI/RenderingSystemValues.h"
#include "Engine/Module/World/Camera/Camera3D.h"

void WorldLayerRenderNode::setup(Data&& data_) {
	data = std::move(data_);
	subtree.setup(data.gBuffer.texture);
}

void WorldLayerRenderNode::stack_command() {
	if (!data.layerData.camera || !data.layerData.camera->is_active()) {
		return;
	}

	auto&& commandList = DxCommand::GetCommandList();
	Reference<DepthStencilTexture> depthStencilTexture = RenderingSystemValues::GetDepthStencilTexture();

	// RenderTargetの設定
	// ViewPortの設定
	commandList->RSSetViewports(1, &data.gBuffer.viewport);
	// シザー矩形の設定
	commandList->RSSetScissorRects(1, &data.gBuffer.rect);
	data.gBuffer.renderTarget->begin_write(true, depthStencilTexture);
	depthStencilTexture->start_write();

	// ----- GBufferPass -----
	// StaticMesh
	subtree.begin_nodes();
	data.layerData.camera->register_world_projection(2);
	data.layerData.worldRenderCollection->staticMeshDrawManager.draw_layer(data.layerData.index);

	// SkinningMesh
	subtree.next_node();
	data.layerData.camera->register_world_projection(2);
	data.layerData.worldRenderCollection->staticMeshDrawManager.draw_layer(data.layerData.index);

	// ----- LightingPass -----
	// ViewPortの設定
	commandList->RSSetViewports(1, &data.gBuffer.viewport);
	// シザー矩形の設定
	commandList->RSSetScissorRects(1, &data.gBuffer.rect);
	data.outputRenderTargetGroup->begin_write(false, depthStencilTexture);

	// NonLightingPixel
	subtree.next_node(); // 自動実行

	// DirectionalLighting
	subtree.next_node();
	data.layerData.camera->register_world_lighting(1);
	data.layerData.worldRenderCollection->directionalLightingExecutors[data.layerData.index].draw_command();

	// PointLighting
	subtree.next_node();
	data.layerData.camera->register_world_lighting(1);
	data.layerData.worldRenderCollection->pointLightingExecutors[data.layerData.index].draw_command();

	// ----- PrimitivePass -----
	// Rect
	subtree.next_node();
	data.outputRenderTargetGroup->begin_write(false, depthStencilTexture);
	data.layerData.camera->register_world_projection(3);
	data.layerData.camera->register_world_lighting(4);
	data.layerData.worldRenderCollection->directionalLightingExecutors[data.layerData.index].set_command(4);
	data.layerData.worldRenderCollection->rect3dDrawManager.draw_layer(data.layerData.index);

	subtree.next_node();
}

const WorldLayerRenderNode::Data& WorldLayerRenderNode::data_imm() const {
	return data;
}
