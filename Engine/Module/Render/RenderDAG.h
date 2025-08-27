#pragma once

#include <memory>
#include <string>
#include <vector>

#include <Library/Utility/Template/Reference.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

#include "Engine/Module/Render/RenderTargetCollection/RenderTargetCollection.h"

class IRenderNode;
class Scene;

class RenderDAG final {
public:
	RenderDAG() = default;
	~RenderDAG() noexcept = default;

	__CLASS_NON_COPYABLE(RenderDAG)

public:
	void setup(const std::string& sceneName, Reference<Scene> scene);

	void render_entry_point() const;

private:
	RenderTargetCollection renderTargetCollection;
	std::vector<std::unique_ptr<IRenderNode>> renderGraph;
};
