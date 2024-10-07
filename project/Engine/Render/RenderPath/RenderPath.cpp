#include "RenderPath.h"

#include <cassert>

#include "Engine/Render/RenderNode/BaseRenderNode.h"

void RenderPath::initialize(std::vector<std::weak_ptr<BaseRenderNode>>&& list) {
	// サイズ0のPathはバグるので止める
	assert(!list.empty());
	renderNodeList = std::move(list);
}

void RenderPath::initialize(std::initializer_list<std::weak_ptr<BaseRenderNode>>&& list) {
	// サイズ0のPathはバグるので止める
	assert(list.size() >= 1);
	// 全て転送
	renderNodeList = std::move(list);
}

void RenderPath::use() {
	// nowNodeのリセットしておく
	nowNode = renderNodeList.end();
	for (auto& renderNode : renderNodeList) {
		//renderNode.lock()->use();
	}
}

bool RenderPath::begin() {
	// nowNodeのリセット
	nowNode = renderNodeList.begin();
	if (nowNode != renderNodeList.end()) {
		// 最初の描画処理を開始
		nowNode->lock()->begin();
		return true;
	}
	else {
		return false;
	}
}

bool RenderPath::next() {
	// 今の処理を終了
	nowNode->lock()->end();
	// 次に進める
	++nowNode;
	// 末尾に行っていなければ次の処理を開始
	if (nowNode != renderNodeList.end()) {
		nowNode->lock()->begin();
		return true;
	}
	else {
		// 終わっていれば処理を終了する
		return false;
	}
}

bool RenderPath::is_end() {
	return nowNode == renderNodeList.end();
}
