#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include <Library/Utility/Template/SingletonInterface.h>

#include <Library/Externals/msdf-atlas-gen/msdf-atlas-gen/msdf-atlas-gen.h>

class FontAtlasBuilderManager final : public SingletonInterface<FontAtlasBuilderManager> {
	__CLASS_SINGLETON_INTERFACE(FontAtlasBuilderManager)

public:
	static void Initialize();
	static void Finalizce();

public:
	static msdfgen::FreetypeHandle* FreetypeHandle();
	static const msdf_atlas::Charset& Charset();

private:
	msdf_atlas::Charset charaset;
	msdfgen::FreetypeHandle* freetype;
};

#endif // DEBUG_FEATURES_ENABLE
