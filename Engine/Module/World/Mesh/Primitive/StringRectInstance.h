#pragma once

#include "./IPrimitiveInstance.h"
#include "./CharRectStruct.h"

#include "Engine/Assets/FontAtlasMSDF/FontAtlasMSDFAsset.h"

struct StringData {
	r32 fondSize;
	Vector2 pivot{ CVector2::ZERO };
	u32 textureIndex;
};

struct CharRectData {
	u32 top;
	u32 left;
	u32 bottom;
	u32 right;
};

class StringRectInstance : public IPrimitiveInstance<StringData> {
public:
	StringRectInstance() noexcept;
	virtual ~StringRectInstance() noexcept;

	__CLASS_NON_COPYABLE(StringRectInstance)

public:
	void initialize(const std::string& msdfFont, r32 fontSize_, const Vector2& pivot_ = CVector2::ZERO);

public:
	void set_font_size(r32 fontSize);
	r32 font_size() const;

	void set_pivot(const Vector2& pivot);
	const Vector2& pivot_imm() const;
	
	void set_string(std::string_view string_);
	const std::string& string_imm() const;

	const std::vector<GlyphRenderingData>& glyph_data_imm() const;

	u32 font_texture_index() const;
	std::optional<u32> glyph_bindless_index() const;

private:
	std::shared_ptr<const FontAtlasMSDFAsset> fontAtlas;
	std::string string;
	std::vector<GlyphRenderingData> charRenderingData;
};
