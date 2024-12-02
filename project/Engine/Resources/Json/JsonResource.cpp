#include "JsonResource.h"

#include <fstream>

#include "Engine/Debug/Output.h"

JsonResource::JsonResource(const std::filesystem::path& file) {
	load(file);
}

void JsonResource::load(const std::filesystem::path& file) {
	assert(file.extension() == ".json");
	const std::filesystem::path DEFAULT_DIRECTORY{ "./Resources/Json/" };

	// 相対ディレクトリで始まる場合
	if (file.string().starts_with("./")) {
		filePath = file;
	}
	// ファイル名のみor一部ディレクトリの続きの場合
	else {
		filePath = DEFAULT_DIRECTORY / file;
	}

	std::ifstream ifstream{ filePath };

	if (ifstream.fail()) {
		Console("[JsonResource] File-\'{}\'", filePath.stem().string());
		return;
	}

	json = nlohmann::json::parse(ifstream);
}

void JsonResource::save() const {
	std::ofstream ofstream{ filePath, std::ios_base::out };
	ofstream << json;
	ofstream.close();
}

nlohmann::json& JsonResource::get() {
	return json;
}

const nlohmann::json& JsonResource::get() const {
	return json;
}
