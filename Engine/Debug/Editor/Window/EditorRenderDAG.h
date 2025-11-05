#pragma once

#include "IEditorWindow.h"

#include <memory>
#include <unordered_map>

#include <ImNodeFlow/include/ImNodeFlow.h>

class EditorRenderDAG final : public IEditorWindow {
public:
	EditorRenderDAG();
	~EditorRenderDAG() override = default;

	__CLASS_NON_COPYABLE(EditorRenderDAG)

public:
	void initialize();
	void finalize();

	void draw() override;

private:
	u64 outPinIdCounter{ 0 };

	std::unique_ptr<ImFlow::ImNodeFlow> imNodeFlow{};
	std::unordered_map<u64, std::shared_ptr<ImFlow::BaseNode>> nodes;
};
