#include "EditorRenderDAG.h"

#include <unordered_set>

#include <ImNodeFlow/include/ImNodeFlow.h>

#include "../Adapter/RenderDAGImNodeLoader.h"
#include "../Adapter/RenderDAGImNodeSaver.h"
#include "./RenderDagImNode/PostEffectImNode.h"
#include "./RenderDagImNode/ResultImNode.h"
#include "./RenderDagImNode/StaticTextureImNode.h"
#include "./RenderDagImNode/WorldLayerRenderImNode.h"
#include "Engine/Application/Logger.h"
#include "Engine/Assets/Json/JsonAsset.h"

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
				auto imNode = imNodeFlow->placeNode<WorldLayerRenderImNode>();
				imNode->set_node_id(nodeCounter);
				nodes.emplace(nodeCounter, imNode);
				++nodeCounter;
			}
			if (ImGui::MenuItem("PostEffect")) {
				auto imNode = imNodeFlow->placeNode<PostEffectImNode>();
				imNode->set_node_id(nodeCounter);
				nodes.emplace(nodeCounter, imNode);
				++nodeCounter;
			}
			if (ImGui::MenuItem("StaticTexture")) {
				auto imNode = imNodeFlow->placeNode<StaticTextureImNode>();
				imNode->set_node_id(nodeCounter);
				nodes.emplace(nodeCounter, imNode);
				++nodeCounter;
			}
		}
	});
	imNodeFlow->droppedLinkPopUpContent([&](ImFlow::Pin* pin) {
		bool isInPin = pin->getType() == ImFlow::PinType::PinType_Input;
		if (ImGui::MenuItem("WorldRender")) {
			auto imNode = imNodeFlow->placeNode<WorldLayerRenderImNode>();
			imNode->set_node_id(nodeCounter);
			if (isInPin) {
				auto& outPin = imNode->getOuts().front();
				outPin->createLink(pin);
			}
			else {
				auto& inPin = imNode->getIns().front();
				pin->createLink(inPin.get());
			}
			nodes.emplace(nodeCounter, imNode);
			++nodeCounter;
		}
		if (ImGui::MenuItem("PostEffect")) {
			auto imNode = imNodeFlow->placeNode<PostEffectImNode>();
			imNode->set_node_id(nodeCounter);
			if (isInPin) {
				auto& outPin = imNode->getOuts().front();
				outPin->createLink(pin);
			}
			else {
				auto& inPin = imNode->getIns().front();
				pin->createLink(inPin.get());
			}
			nodes.emplace(nodeCounter, imNode);
			++nodeCounter;
		}
		if (ImGui::MenuItem("StaticTexture")) {
			auto imNode = imNodeFlow->placeNode<StaticTextureImNode>();
			imNode->set_node_id(nodeCounter);
			if (isInPin) {
				auto& outPin = imNode->getOuts().front();
				outPin->createLink(pin);
			}
			else {
				auto& inPin = imNode->getIns().front();
				pin->createLink(inPin.get());
			}
			nodes.emplace(nodeCounter, imNode);
			++nodeCounter;
		}
	});
	generate_result_node();
	nodeCounter = 1;
}

void EditorRenderDAG::setup(const std::string& sceneName) {
	load(sceneName);
}

void EditorRenderDAG::finalize() {
	imNodeFlow.reset();
}

void EditorRenderDAG::load(const std::string& sceneName) {
	JsonAsset input{ "./Game/Core/Scene/" + sceneName + "/RenderPath.json" };
	for (auto& [key, value] : input.cget().items()) {

	}
}

void EditorRenderDAG::save(const std::string& sceneName) {
	std::unordered_map<u64, bool> seen;
	for (auto& [id_, _] : nodes) {
		seen[id_] = false;
	}
	std::vector<DAGNodeType> sortedNodes;
	std::unordered_set<u64> visiting;

	auto sort_dag = [&](u64 id, auto&& sort_dag_ref) {
		if (!nodes.contains(id)) {
			szgWarning("Node id {} is missing in EditorRenderDAG::save()", id);
			return;
		}

		if (visiting.contains(id)) {
			szgWarning("Cycle detected in RenderDAG in EditorRenderDAG::save()");
			return;
		}

		if (seen[id]) {
			return;
		}
		seen[id] = true;
		auto& node = nodes.at(id);

		visiting.emplace(id);

		for (auto& in : std::visit([](const std::shared_ptr<ImFlow::BaseNode>& n) { return n->getIns(); }, node)) {
			if (!in->isConnected()) {
				szgInformation("Input pin is not connected in EditorRenderDAG::save()");
				continue;
			}
			std::shared_ptr<ImFlow::Link> link = in->getLink().lock();
			if (!link) {
				szgWarning("Failed to lock link in EditorRenderDAG::save()");
				continue;
			}
			IRenderDagImNode* outPin = static_cast<IRenderDagImNode*>(link->left()->getParent());
			u64 nextOutId = outPin->node_id();
			sort_dag_ref(nextOutId, sort_dag_ref);
		}

		visiting.erase(id);
		sortedNodes.emplace_back(node);
	};

	sort_dag(0, sort_dag);

	// サイクルチェック
	if (!visiting.empty()) {
		szgError("RenderDAG contains cycle(s), cannot save.");
		return;
	}

	sortedNodes.pop_back(); // Remove Result node

	// ノードIDからソート済みインデックスへの変換テーブルを作成
	std::unordered_map<u64, u64> sortedNodesInverse;
	for (u64 i = 0; const DAGNodeType& node : sortedNodes) {
		u64 nodeId = std::visit([](const std::shared_ptr<IRenderDagImNode>& n) { return n->node_id(); }, node);
		sortedNodesInverse[nodeId] = i;
		++i;
	}

	// ノードの出力先を収集
	std::unordered_map<u64, std::vector<u64>> outputTargetNodeIndices;
	for (auto& [id, node] : nodes) {
		for (auto& inPin : std::visit([](const std::shared_ptr<ImFlow::BaseNode>& n) { return n->getIns(); }, node)) {
			if (!inPin->isConnected()) {
				continue;
			}
			auto link = inPin->getLink().lock();
			if (!link) {
				continue;
			}
			u64 inNodeId = static_cast<IRenderDagImNode*>(link->right()->getParent())->node_id();
			u64 outNodeId = static_cast<IRenderDagImNode*>(link->left()->getParent())->node_id();
			outputTargetNodeIndices[outNodeId].emplace_back(sortedNodesInverse[inNodeId]);
		}
	}

	JsonAsset output{ "./Game/Core/Scene/" + sceneName + "/RenderPath.json" }; // 書き出し用ファイル
	// 既にある内容を削除
	output.get().clear();

	for (auto& node : sortedNodes) {
		// ノードのベース情報をJson化
		nlohmann::json nodeJson = std::visit(RenderDAGImNodeSaver{}, node);
		// 接続状況の保存
		nodeJson["Links"] = nlohmann::json::array();
		nlohmann::json& linksJson = nodeJson["Links"];
		u64 nodeId = std::visit([](const std::shared_ptr<IRenderDagImNode>& n) { return n->node_id(); }, node);
		for (u64 id : outputTargetNodeIndices[nodeId]) {
			linksJson.emplace_back(id);
		}
		// JsonAssetに追加
		output.get().emplace_back(nodeJson);
	}
	// ファイル書き出し
	output.save();
}

void EditorRenderDAG::draw() {
	if (!isActive) {
		return;
	}

	int flags = 0;
	ImGui::Begin("Render DAG Editor", &isActive, flags);

	if (ImGui::Button("Save")) {
		save("EditorTestScene");
	}

	if (!nodes.contains(0)) {
		szgWarning("Don't delete ScreenOutNode!");
		generate_result_node();
	}

	std::erase_if(nodes, [](const std::pair<u64, DAGNodeType>& pair) {
		return std::visit([](const std::shared_ptr<ImFlow::BaseNode>& node) { return node->toDestroy(); }, pair.second);
	});

	imNodeFlow->setSize(ImGui::GetContentRegionAvail());
	imNodeFlow->update();

	ImGui::End();
}

void EditorRenderDAG::generate_result_node() {
	auto resultNode = imNodeFlow->addNode<ResultImNode>({ 600, 350 });
	nodes.emplace(0, resultNode);
	resultNode->set_node_id(0);
}
