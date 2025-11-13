#include "PostEffectImNode.h"

enum class PostEffectType {
	None,
	ChromaticAberration,
	Grayscale,
	Outline,
	RadialBlur,
	DownSampling,
	TextureBlend2,
	TextureBlend4,
};

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

	if (ImGui::BeginCombo("Type", peName[data.peType.cget()].c_str())) {
		for (u32 i = 0; i < peName.size(); ++i) {
			bool is_selected = data.peType.cget() == i;
			if (ImGui::Selectable(peName[i].c_str(), is_selected)) {
				if (is_selected) {
					continue;
				}
				if (data.peType.cget() == static_cast<u32>(PostEffectType::TextureBlend2)) {
					dropIN("MixTexture");
				}
				else if (data.peType.cget() == static_cast<u32>(PostEffectType::TextureBlend4)) {
					dropIN("MixTexture1");
					dropIN("MixTexture2");
					dropIN("MixTexture3");
				}
				else {
				}
				data.peType = i;
				update_extra_input_pins();
			}
		}
		ImGui::EndCombo();
	}

	ImGui::PopItemWidth();
}

void PostEffectImNode::update_extra_input_pins() {
	if (data.peType.cget() == static_cast<u32>(PostEffectType::TextureBlend2)) {
		data.extraInputs.resize(1, 0);
		addIN<u64>("MixTexture", 0ull, ImFlow::ConnectionFilter::SameType());
	}
	else if (data.peType.cget() == static_cast<u32>(PostEffectType::TextureBlend4)) {
		data.extraInputs.resize(3, 0);
		addIN<u64>("MixTexture1", 0ull, ImFlow::ConnectionFilter::SameType());
		addIN<u64>("MixTexture2", 0ull, ImFlow::ConnectionFilter::SameType());
		addIN<u64>("MixTexture3", 0ull, ImFlow::ConnectionFilter::SameType());
	}
	else {
		data.extraInputs.clear();
	}
}

const PostEffectImNode::Data& PostEffectImNode::get_data() const {
	return data;
}

void PostEffectImNode::set_data(const Data& data_) {
	data = data_;
}
