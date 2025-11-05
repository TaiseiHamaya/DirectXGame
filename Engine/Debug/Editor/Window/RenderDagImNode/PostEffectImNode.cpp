#include "PostEffectImNode.h"

std::vector<std::string> PostEffectImNode::peName{
	"None",
	"色収差",
	"グレースケール",
	"アウトライン",
	"ラジアルブラー",
};

PostEffectImNode::PostEffectImNode(u64 outPinId_) {
	outPinId = outPinId_;
	setTitle("PostEffect");
	setStyle(ImFlow::NodeStyle::cyan());

	addIN<u64>("Base", 0, ImFlow::ConnectionFilter::SameType());
	addOUT<u64>("Output", nullptr)->behaviour([&]() {
		return outPinId;
	});
}

void PostEffectImNode::draw() {
	ImGui::PushItemWidth(120.f);
	outputSize.show_gui();

	ImGui::Separator();

	if (ImGui::BeginCombo("Type", peName[peType.cget()].c_str())) {
		for (u32 i = 0; i < peName.size(); ++i) {
			bool is_selected = peType.cget() == i;
			if (ImGui::Selectable(peName[i].c_str(), is_selected)) {
				peType = i;
			}
		}
		ImGui::EndCombo();
	}

	ImGui::PopItemWidth();
}
