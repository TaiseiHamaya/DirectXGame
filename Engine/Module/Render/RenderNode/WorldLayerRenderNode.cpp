#include "WorldLayerRenderNode.h"

#include "../WorldRenderCollection.h"
#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"
#include "Engine/GraphicsAPI/RenderingSystemValues.h"
#include "Engine/Module/World/Camera/Camera3D.h"

void WorldLayerRenderNode::initialize() {
	std::array<Reference<RenderTexture>, DeferredAdaptor::NUM_GBUFFER> tempGBuffer;
	for (u32 ti = 0; ti < DeferredAdaptor::NUM_GBUFFER; ++ti) {
		gBuffer.texture[ti].initialize(DeferredAdaptor::DXGI_FORMAT_LIST[ti]);
		tempGBuffer[ti] = gBuffer.texture[ti];
	}

	subtree.setup(tempGBuffer);
}

void WorldLayerRenderNode::setup(Reference<WorldRenderCollection> worldRenderCollection_) {
	worldRenderCollection = worldRenderCollection_;
}

void WorldLayerRenderNode::stack_command() {
	if (!camera || !camera->is_active()) {
		return;
	}

	auto&& commandList = DxCommand::GetCommandList();
	Reference<DepthStencilTexture> depthStencilTexture = RenderingSystemValues::GetDepthStencilTexture();

	// RenderTargetの設定
	// ViewPortの設定
	commandList->RSSetViewports(1, &gBuffer.viewport);
	// シザー矩形の設定
	commandList->RSSetScissorRects(1, &gBuffer.rect);
	gBuffer.renderTarget.begin_write(true, depthStencilTexture);
	depthStencilTexture->start_write();

	// ----- GBufferPass -----
	// StaticMesh
	subtree.begin_nodes();
	camera->register_world_projection(2);
	worldRenderCollection->staticMeshDrawManager.draw_layer(renderLayer);

	// SkinningMesh
	subtree.next_node();
	camera->register_world_projection(2);
	worldRenderCollection->staticMeshDrawManager.draw_layer(renderLayer);

	// ----- LightingPass -----
	// ViewPortの設定
	commandList->RSSetViewports(1, &viewport);
	// シザー矩形の設定
	commandList->RSSetScissorRects(1, &rect);
	outputRenderTargetGroup->begin_write(false);

	// NonLightingPixel
	subtree.next_node(); // 自動実行

	// DirectionalLighting
	subtree.next_node();
	camera->register_world_lighting(1);
	worldRenderCollection->directionalLightingExecutors[renderLayer].draw_command();

	// PointLighting
	subtree.next_node();
	camera->register_world_lighting(1);
	worldRenderCollection->pointLightingExecutors[renderLayer].draw_command();

	// ----- PrimitivePass -----
	// Rect
	subtree.next_node();
	outputRenderTargetGroup->begin_write(false, depthStencilTexture);
	camera->register_world_projection(3);
	camera->register_world_lighting(4);
	worldRenderCollection->directionalLightingExecutors[renderLayer].set_command(4);
	worldRenderCollection->rect3dDrawManager.draw_layer(renderLayer);

	subtree.next_node();
}
