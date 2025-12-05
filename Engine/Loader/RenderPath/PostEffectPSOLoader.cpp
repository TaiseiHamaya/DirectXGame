#include "PostEffectPSOLoader.h"

#include "Engine/Application/Logger.h"
#include "Engine/GraphicsAPI/RenderingSystemValues.h"
#include "Engine/Loader/RenderPath/RenderNodeType.h"
#include "Engine/Module/Manager/RuntimeStorage/RuntimeStorage.h"
#include "Engine/Module/Render/RenderPipeline/Posteffect/ChromaticAberration/ChromaticAberrationPipeline.h"
#include "Engine/Module/Render/RenderPipeline/Posteffect/Grayscale/GrayscalePipeline.h"
#include "Engine/Module/Render/RenderPipeline/Posteffect/Outline/OutlinePipeline.h"
#include "Engine/Module/Render/RenderPipeline/Posteffect/RadialBlur/RadialBlurPipeline.h"

#define VECTOR2_SERIALIZER
#include "Engine/Assets/Json/JsonSerializer.h"

void PostEffectPSOLoader::setup(Reference<std::vector<RenderNodeLoader::ImmidiateData>> immediateData_) {
	immediateData = immediateData_;
}

std::unique_ptr<IPostEffectPipeline> PostEffectPSOLoader::entry_point(const nlohmann::json& json) {
	RuntimeStorage::ValueGroup& postEffectValueGroup = RuntimeStorage::GetValueList("PostEffect");
	std::any value;
	std::unique_ptr<IPostEffectPipeline> node = nullptr;

	const nlohmann::json& dataJson = json["Data"];
	const nlohmann::json& linkJson = json["Links"];
	const PostEffectType type = dataJson["Type"].get<PostEffectType>();
	switch (type) {
	case PostEffectType::ChromaticAberration:
	{
		auto temp = std::make_unique<ChromaticAberrationPipeline>();
		temp->initialize();
		temp->set_shader_texture(immediateData->at(linkJson["Base"]).renderTexture);
		if (!dataJson["EffectTag"].is_null()) {
			temp->setup(dataJson.value("EffectTag", ""));
		}
		node = std::move(temp);
		if (dataJson.value("UseRuntime", false)) {
			value = CVector2::ZERO;
		}
	}
	break;
	case PostEffectType::Grayscale:
	{
		auto temp = std::make_unique<GrayscalePipeline>();
		temp->initialize();
		temp->set_shader_texture(immediateData->at(linkJson["Base"]).renderTexture);
		node = std::move(temp);
		if (dataJson.value("UseRuntime", false)) {
			value = false;
		}
	}
	break;
	case PostEffectType::Outline:
	{
		auto temp = std::make_unique<OutlinePipeline>();
		temp->initialize();
		temp->set_shader_texture(
			immediateData->at(linkJson["Base"]).renderTexture,
			RenderingSystemValues::GetDepthStencilTexture()
		);
		node = std::move(temp);
		if (dataJson.value("UseRuntime", false)) {
			// do nothing
		}
	}
	break;
	case PostEffectType::RadialBlur:
	{
		auto temp = std::make_unique<RadialBlurPipeline>();
		temp->initialize();
		temp->set_shader_texture(immediateData->at(linkJson["Base"]).renderTexture);
		if (dataJson.value("UseRuntime", false)) {
			value = BlurInfo{};
		}
	}
	break;
	case PostEffectType::DownSampling:
		break;
	case PostEffectType::TextureBlend2:
		break;
	case PostEffectType::TextureBlend4:
		break;
	default:
		szgWarning("PostEffectPSOLoader::entry_point: Unknown PostEffectType \'{}\'", static_cast<i32>(type));
		return nullptr;
	}

	if (value.has_value()) {
		postEffectValueGroup[dataJson["EffectTag"].get<std::string>()] = std::move(value);
	}

	return node;
}
