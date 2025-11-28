#ifdef DEBUG_FEATURES_ENABLE

#include "FontAtlasBuilderManager.h"

void FontAtlasBuilderManager::Initialize() {
	auto& instance = GetInstance();
	if (instance.freetype) {
		return;
	}
	instance.freetype = msdfgen::initializeFreetype();
	instance.charaset.load("./DirectXGame/EditorResources/charset.txt");
}

void FontAtlasBuilderManager::Finalizce() {
	auto& instance = GetInstance();
	deinitializeFreetype(instance.freetype);
}

msdfgen::FreetypeHandle* FontAtlasBuilderManager::FreetypeHandle() {
	auto& instance = GetInstance();
	return instance.freetype;
}

const msdf_atlas::Charset& FontAtlasBuilderManager::Charset() {
	auto& instance = GetInstance();
	return instance.charaset;
}

#endif // DEBUG_FEATURES_ENABLE
