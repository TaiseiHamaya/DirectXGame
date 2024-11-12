#pragma once

#include <unordered_map>
#include <string>
#include <optional>
#include <filesystem>

struct EnemyTypeData;

class EnemyTypeEditor {
public:
	void initialize();
	void debug_gui();

public:
	const EnemyTypeData& get_template(const std::string& name) const;
	const std::unordered_map<std::string, EnemyTypeData>& template_data() const { return templateData; };

private:
	void reset_select(std::optional<std::string> select_ = std::nullopt);
	void detail_window();
	void obj_file_list();
	void get_file_list(std::vector<std::string>& list, const std::filesystem::path& path, const std::string& extension);

private:
	void load_json();
	void export_json_all();
	void load_obj();

private:
	static const inline std::filesystem::path LoadPath
	{ "./Resources/GameScene/EnemyTemplate/" };

	std::optional<std::string> select;

	std::string* searchResultAddress;
	std::filesystem::path meshCurrentPath;
	std::vector<std::string> meshFileList;

	std::unordered_map<std::string, EnemyTypeData> templateData;
};
