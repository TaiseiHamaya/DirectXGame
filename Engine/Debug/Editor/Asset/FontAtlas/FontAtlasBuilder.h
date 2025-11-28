#pragma once

#include <filesystem>

class FontAtlasBuilder final {
public:
	void entry_point(const std::filesystem::path& path);
};
