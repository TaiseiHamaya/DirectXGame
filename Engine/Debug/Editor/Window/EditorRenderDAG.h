#pragma once

#include "IEditorWindow.h"

#include <filesystem>
#include <memory>
#include <unordered_map>
#include <variant>

#include <ImNodeFlow/include/ImNodeFlow.h>

class WorldLayerRenderImNode;
class PostEffectImNode;
class StaticTextureImNode;
class ResultImNode;

class EditorRenderDAG final : public IEditorWindow {
public:
	EditorRenderDAG();
	~EditorRenderDAG() override = default;

	__CLASS_NON_COPYABLE(EditorRenderDAG)

public:
	using DAGNodeType =
		std::variant<
		std::shared_ptr<WorldLayerRenderImNode>,
		std::shared_ptr<PostEffectImNode>,
		std::shared_ptr<StaticTextureImNode>,
		std::shared_ptr<ResultImNode>
		>;

public:
	void initialize();
	void setup(const std::string& sceneName);
	void finalize();

	void draw() override;

private:
	void generate_result_node();

private:
	u64 nodeCounter{ 0 };

	std::unique_ptr<ImFlow::ImNodeFlow> imNodeFlow{};
	std::unordered_map<u64, DAGNodeType> nodes;
};
