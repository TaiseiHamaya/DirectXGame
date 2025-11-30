#pragma once

#include <memory>
#include <vector>

#include <json.hpp>

#include <Library/Utility/Template/Reference.h>

class Scene;
class RenderTargetCollection;
class IRenderNode;
class BaseRenderTargetGroup;
class RenderTexture;
class PostEffectPSOLoader;

class RenderNodeLoader {
public:
	struct ImmidiateData {
		Reference<BaseRenderTargetGroup> renderTargetGroup;
		Reference<RenderTexture> renderTexture;
	};

public:
	RenderNodeLoader();
	~RenderNodeLoader() noexcept;

public:
	void setup(Reference<Scene> scene_, Reference<RenderTargetCollection> renderTargetCollection);
	std::vector<std::unique_ptr<IRenderNode>> entry_point(const nlohmann::json& json);

private:
	std::unique_ptr<IRenderNode> load_as_post_effect(const nlohmann::json& json);
	std::unique_ptr<IRenderNode> load_as_world_render(const nlohmann::json& json);
	void load_as_static_texture(const nlohmann::json& json);

	void calclate_result_node_index(const nlohmann::json& json);

private:
	std::vector<std::unique_ptr<IRenderNode>> result;

	Reference<Scene> scene;
	Reference<RenderTargetCollection> renderTargetCollection;

	std::unique_ptr<PostEffectPSOLoader> pePsoLoader;

	std::vector<ImmidiateData> immidiateData;
	i32 resultNodeIndex{ 0 };
};
