#include "RenderPath.h"

#include <format>

#include "Engine/DirectX/PipelineState/PipelineState.h"
#include "Engine/Render/RenderNode/BaseRenderNode.h"
#include "Engine/DirectX/DirectXCore.h"
#include "Engine/Utility/Utility.h"

void RenderPath::initialize(std::vector<std::weak_ptr<BaseRenderNode>>&& list) {
	renderNodeList = std::move(list);
}

void RenderPath::initialize(std::initializer_list<std::weak_ptr<BaseRenderNode>>&& list) {
	renderNodeList.reserve(list.size());
	for (auto listItr = list.begin(); listItr != list.end(); ++listItr) {
		renderNodeList.emplace_back(std::move(*listItr));
	}
}

bool RenderPath::begin() {
	nowNode = renderNodeList.begin();
	if (nowNode != renderNodeList.end()) {
		nowNode->lock()->begin();
		return true;
	}
	else {
		DirectXCore::SetScreenRenderTarget();
		return false;
	}
}

bool RenderPath::next() {
	nowNode->lock()->end();
	++nowNode;
	if (nowNode != renderNodeList.end()) {
		nowNode->lock()->begin();
		return true;
	}
	else {
		DirectXCore::SetScreenRenderTarget();
		return false;
	}
}
