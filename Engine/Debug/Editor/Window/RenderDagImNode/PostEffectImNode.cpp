#include "PostEffectImNode.h"

#ifdef DEBUG_FEATURES_ENABLE

#include "Engine/Module/Render/RenderPSO/PostEffectPSO.h"

std::vector<std::string> PostEffectImNode::peName{
	"None",
	"色収差",
	"グレースケール",
	"アウトライン",
	"ラジアルブラー",
	"ダウンサンプリング",
	"テクスチャ合成2",
	"テクスチャ合成4",
};

PostEffectImNode::PostEffectImNode() {
	setTitle("PostEffect");
	setStyle(ImFlow::NodeStyle::cyan());

	addIN<u64>("Base", 0, ImFlow::ConnectionFilter::SameType());
	addOUT<u64>("Output", nullptr)->behaviour([&]() {
		return nodeId;
	});
}

void PostEffectImNode::draw() {
	ImGui::PushItemWidth(120.f);
	data.outputSize.show_gui();
	data.isUseRuntime.show_gui();

	if (data.isUseRuntime) {
		data.effectTagName.show_gui();
	}

	if (ImGui::BeginCombo("Type", peName[data.peType.cget()].c_str())) {
		for (u32 i = 0; i < peName.size(); ++i) {
			bool isSelected = data.peType.cget() == i;
			if (ImGui::Selectable(peName[i].c_str(), isSelected)) {
				if (isSelected) {
					continue;
				}
				remove_extra_input_pins();
				data.peType = i;
				update_extra_input_pins();
			}
		}
		ImGui::EndCombo();
	}

	ImGui::PopItemWidth();
}

void PostEffectImNode::remove_extra_input_pins() {
	switch (static_cast<PostEffectType>(data.peType.cget())) {
	case PostEffectType::TextureBlend2:
		dropIN("MixTexture");
		break;
	case PostEffectType::TextureBlend4:
		dropIN("MixTexture1");
		dropIN("MixTexture2");
		dropIN("MixTexture3");
		break;
	default:
		break;
	}
}

void PostEffectImNode::update_extra_input_pins() {
	switch (static_cast<PostEffectType>(data.peType.cget())) {
	case PostEffectType::TextureBlend2:
		data.extraInputs.resize(1, 0);
		addIN<u64>("MixTexture", 0ull, ImFlow::ConnectionFilter::SameType());
		break;
	case PostEffectType::TextureBlend4:
		data.extraInputs.resize(3, 0);
		addIN<u64>("MixTexture1", 0ull, ImFlow::ConnectionFilter::SameType());
		addIN<u64>("MixTexture2", 0ull, ImFlow::ConnectionFilter::SameType());
		addIN<u64>("MixTexture3", 0ull, ImFlow::ConnectionFilter::SameType());
		break;
	default:
		data.extraInputs.clear();
		break;
	}
}

const PostEffectImNode::Data& PostEffectImNode::get_data() const {
	return data;
}

void PostEffectImNode::set_data(const Data& data_) {
	data = data_;
}

#endif // DEBUG_FEATURES_ENABLE
