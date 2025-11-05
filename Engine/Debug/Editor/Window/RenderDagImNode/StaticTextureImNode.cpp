#include "StaticTextureImNode.h"

StaticTextureImNode::StaticTextureImNode(u64 outPinId_) {
	outPinId = outPinId_;
	setTitle("StaticTexture");
	setStyle(ImFlow::NodeStyle::green());
	addOUT<u64>("Output", nullptr)->behaviour([&]() {
		return outPinId;
	});
}

void StaticTextureImNode::draw() {
	ImGui::PushItemWidth(120.f);
	textureFilename.show_gui();
	ImGui::PopItemWidth();
}
