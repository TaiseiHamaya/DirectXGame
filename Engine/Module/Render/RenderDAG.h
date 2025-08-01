#pragma once

#include <memory>
#include <string>
#include <vector>

#include <json.hpp>

#include <Library/Utility/Tools/ConstructorMacro.h>

#include "Engine/Module/Render/RenderTargetCollection/RenderTargetCollection.h"

class IRenderNode;

class RenderDAG final {
public:
	RenderDAG() = default;
	~RenderDAG() noexcept = default;

	__CLASS_NON_COPYABLE(RenderDAG)

public:
	void setup(const std::string& sceneName);

	void render_entry_point() const;

private:
	void load_render_graph(const nlohmann::json& json);
	// vertex生成関数

private:
	RenderTargetCollection renderTargetCollection;
	std::vector<std::unique_ptr<IRenderNode>> renderGraph;
};
