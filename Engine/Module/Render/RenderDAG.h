#pragma once

#include <filesystem>
#include <list>
#include <memory>
#include <vector>

#include <Library/Utility/Tools/ConstructorMacro.h>

class IRenderVertex;

class RenderDAG final {
public:
	RenderDAG() = default;
	~RenderDAG() noexcept = default;

	__CLASS_NON_COPYABLE(RenderDAG)

public:
	void setup(std::filesystem::path sceneName);

	void render_entry_point();

private:
	struct RenderVertexBase {
		std::unique_ptr<IRenderVertex> vertex;
		std::list<u32> nextVertices;
	};

private:
	void load_render_graph(const std::filesystem::path& sceneName);
	// vertex生成関数

	// DAGをトポロジカルソート
	void sort_render_graph();

private:
	std::vector<std::unique_ptr<IRenderVertex>> renderGraph;
};
