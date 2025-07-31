#pragma once

#include <filesystem>
#include <list>
#include <memory>
#include <vector>

#include <Library/Utility/Tools/ConstructorMacro.h>

class IRenderNode;

class RenderDAG final {
public:
	RenderDAG() = default;
	~RenderDAG() noexcept = default;

	__CLASS_NON_COPYABLE(RenderDAG)

public:
	void setup(std::filesystem::path sceneName);

	void render_entry_point() const;

private:
	struct RenderNodeBase {
		std::unique_ptr<IRenderNode> vertex;
		std::list<u32> nextVertices;
	};

private:
	void load_render_graph(const std::filesystem::path& sceneName);
	// vertex生成関数

	// DAGをトポロジカルソート
	void sort_render_graph();

private:
	std::vector<std::unique_ptr<IRenderNode>> renderGraph;
};
