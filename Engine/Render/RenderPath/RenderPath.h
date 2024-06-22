#pragma once

#include <vector>
#include <memory>
#include <initializer_list>

class BaseRenderNode;

class RenderPath {
public:
	RenderPath() = default;
	~RenderPath() noexcept = default;

	RenderPath(const RenderPath&) = delete;
	RenderPath& operator=(const RenderPath&) = delete;
	RenderPath(RenderPath&&) = default;
	RenderPath& operator=(RenderPath&&) = default;

public:
	void initialize(std::vector<std::weak_ptr<BaseRenderNode>>&& list);
	void initialize(std::initializer_list<std::weak_ptr<BaseRenderNode>>&& list);
	bool begin();
	bool next();

private:
	std::vector<std::weak_ptr<BaseRenderNode>>::iterator nowNode;
	std::vector<std::weak_ptr<BaseRenderNode>> renderNodeList;
};
