#include "WorldRoot.h"

#include "Engine/Module/Render/RenderNode/Forward/Mesh/StaticMeshNodeForward.h"
#include "Engine/Module/Render/RenderNode/Forward/Mesh/SkinningMeshNodeForward.h"

WorldRoot::WorldRoot() = default;
WorldRoot::~WorldRoot() = default;

void WorldRoot::initialize() {
	renderTexture.initialize(DXGI_FORMAT_R8G8B8A8_UNORM);
	renderTarget.initialize(renderTexture);

	std::shared_ptr<StaticMeshNodeForward> staticMeshNode = std::make_shared<StaticMeshNodeForward>();
	staticMeshNode->initialize();
	staticMeshNode->set_render_target(renderTarget);
	staticMeshNode->set_config(RenderNodeConfig::Default);

	std::shared_ptr<SkinningMeshNodeForward> skinningMeshNode = std::make_shared<SkinningMeshNodeForward>();
	skinningMeshNode->initialize();
	skinningMeshNode->set_render_target(renderTarget);
	skinningMeshNode->set_config(RenderNodeConfig::NoClearDepth | RenderNodeConfig::NoClearRenderTarget);

	renderPath.initialize({
		staticMeshNode, skinningMeshNode
	});

	staticMeshDrawManager.initialize(1);
	skinningMeshDrawManager.initialize(1);
}

void WorldRoot::update() {
	//for (auto& instance : worldInstances | std::views::values) {
	//	instance->instance->begin();
	//}
	//for (auto& instance : worldInstances | std::views::values) {
	//	instance->update();
	//}
	//for (auto& instance : worldInstances | std::views::values) {
	//	instance->late_update();
	//}
}

void WorldRoot::transfer() {
	staticMeshDrawManager.transfer();
	skinningMeshDrawManager.transfer();
}

void WorldRoot::draw() {
	renderPath.begin();
	// StaticMesh
	staticMeshDrawManager.draw_layer(0);

	renderPath.next();
	// SkinningMesh
	skinningMeshDrawManager.draw_layer(0);
}

void WorldRoot::post_update() {
	// Destroy instances
	for(auto& instance : destroyInstances) {
		Reference<const WorldInstance> parent = instance->get_parent_address();
		//worldInstances.at(instance->get_instance_id())->destroyCallback();
	}
}

void WorldRoot::destroy(Reference<WorldInstance> instance) {
	destroyInstances.emplace_back(instance);
}
