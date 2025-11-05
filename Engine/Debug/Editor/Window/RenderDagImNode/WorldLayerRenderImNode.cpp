#include "WorldLayerRenderImNode.h"

WorldLayerRenderImNode::WorldLayerRenderImNode(u64 outPinId_) {
	outPinId = outPinId_;

	setTitle("WorldLayerRender");
	setStyle(ImFlow::NodeStyle::red());

	addIN<u64>("Base", 0, ImFlow::ConnectionFilter::SameType());
	addOUT<u64>("Output", nullptr)->behaviour([&]() {
		return outPinId;
	});
}

void WorldLayerRenderImNode::draw() {
	ImGui::PushItemWidth(120.0f);
	ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_SpanAllColumns | ImGuiTreeNodeFlags_SpanLabelWidth;

	outputSize.show_gui();

	ImGui::Text("World/Layer");

	worldIndex.show_gui();
	layerIndex.show_gui();
	cameraId.show_gui();

	ImGui::Text("GBuffer");

	gBufferSize.show_gui();

	ImGui::Text("Rendering");

	if (ImGui::TreeNodeEx("Viewport", treeNodeFlags)) {
		gBufferViewport.left.show_gui();
		gBufferViewport.top.show_gui();
		gBufferViewport.width.show_gui();
		gBufferViewport.height.show_gui();
		gBufferViewport.minDepth.show_gui();
		gBufferViewport.maxDepth.show_gui();

		ImGui::TreePop();
	}
	
	if (ImGui::TreeNodeEx("Scissor", treeNodeFlags)) {
		layerScissor.topLeft.show_gui();
		layerScissor.bottomRight.show_gui();
		ImGui::TreePop();
	}

	ImGui::PopItemWidth();
}
