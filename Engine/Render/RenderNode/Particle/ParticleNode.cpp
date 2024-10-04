#include "ParticleNode.h"

#include "Engine/DirectX/DirectXCore.h"
#include "Engine/DirectX/DirectXSwapChain/DirectXSwapChain.h"
#include "Engine/DirectX/PipelineState/PipelineState.h"
#include "Engine/DirectX/PipelineState/PSOBuilder/PSOBuilder.h"
#include "Engine/Render/RenderTargetGroup/SingleRenderTarget.h"
#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"

ParticleNode::ParticleNode() = default;

ParticleNode::~ParticleNode() noexcept = default;

void ParticleNode::initialize() {
	create_pipeline_state();
	primitiveTopology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

void ParticleNode::create_pipeline_state() {
}
