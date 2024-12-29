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
		Console("Warning : File-\'{}\' is not found.\n", filePath.stem().string());
		return;
	}

	json = nlohmann::json::parse(ifstream);
}

void JsonResource::save() const {
#ifdef _DEBUG
	valueEditor.save_all();
#endif // _DEBUG

	auto parentPath = filePath.parent_path();
	if (!parentPath.empty() && !std::filesystem::exists(parentPath)) {
		std::filesystem::create_directories(parentPath);
	}

	std::ofstream ofstream{ filePath, std::ios_base::out };
	ofstream << std::setw(1) << std::setfill('\t') << json;
	ofstream.close();
}

nlohmann::json& JsonResource::get() {
	return json;
}

const nlohmann::json& JsonResource::cget() const {
	return json;
}

#ifdef _DEBUG
void JsonResource::show_imgui() {
	valueEditor.show_imgui();
}
#endif // _DEBUG
