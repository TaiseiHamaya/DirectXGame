#pragma once

#include <memory>
#include <vector>

#include <json.hpp>

#include <Library/Utility/Template/Reference.h>

#include "./PostEffectPSOLoader.h"

class Scene;
class RenderTargetCollection;
class IRenderNode;

class RenderNodeLoader {
public:
	void setup(Reference<Scene> scene_, Reference<RenderTargetCollection> renderTargetCollection);
	std::vector<std::unique_ptr<IRenderNode>> entry_point(const nlohmann::json& json);

private:
	std::unique_ptr<IRenderNode> load_post_effect_node(const nlohmann::json& json);
	std::unique_ptr<IRenderNode> load_world_lender_layer_node(const nlohmann::json& json);

private:
	Reference<Scene> scene;
	Reference<RenderTargetCollection> renderTargetCollection;

	PostEffectPSOLoader pePsoLoader;
};
