#include "EditorRenderDAG.h"

#include <ImNodeFlow/include/ImNodeFlow.h>

#include "Engine/Application/Logger.h"
#include "RenderDagImNode/PostEffectImNode.h"
#include "RenderDagImNode/ResultImNode.h"
#include "RenderDagImNode/StaticTextureImNode.h"
#include "RenderDagImNode/WorldLayerRenderImNode.h"

EditorRenderDAG::EditorRenderDAG() {
	isActive = true;
}

void EditorRenderDAG::initialize() {
	imNodeFlow = std::make_unique<ImFlow::ImNodeFlow>();
	imNodeFlow->rightClickPopUpContent([&](ImFlow::BaseNode* node) {
		if (node) { // ノードにホバー時
			if (ImGui::MenuItem("Delete")) {
				if (node->getName() == "ScreenOut") {
					szgWarning("Don't delete ScreenOutNode!");
				}
				else {
					node->destroy();
				}
			}
		}
		else {
			if (ImGui::MenuItem("WorldRender")) {
				auto imNode = imNodeFlow->placeNode<WorldLayerRenderImNode>(outPinIdCounter);
				nodes.emplace(outPinIdCounter, imNode);
				++outPinIdCounter;
			}
			if (ImGui::MenuItem("PostEffect")) {
				auto imNode = imNodeFlow->placeNode<PostEffectImNode>(outPinIdCounter);
				nodes.emplace(outPinIdCounter, imNode);
				++outPinIdCounter;
			}
			if (ImGui::MenuItem("StaticTexture")) {
				auto imNode = imNodeFlow->placeNode<StaticTextureImNode>(outPinIdCounter);
				nodes.emplace(outPinIdCounter, imNode);
				++outPinIdCounter;
			}
		}
	});
	imNodeFlow->addNode<ResultImNode>({ 600, 350 });
}

void EditorRenderDAG::finalize() {
	imNodeFlow.reset();
}

void EditorRenderDAG::draw() {
	if (!isActive) {
		return;
	}

	int flags = 0;
	ImGui::Begin("Render DAG Editor", &isActive, flags);

	imNodeFlow->setSize(ImGui::GetContentRegionAvail());
	imNodeFlow->update();

	ImGui::End();
}
