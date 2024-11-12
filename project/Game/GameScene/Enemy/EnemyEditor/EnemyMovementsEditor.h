#pragma once

#include <unordered_map>
#include <string>
#include <optional>
#include <filesystem>

struct EnemyMovementsData;

class EnemyMovementsEditor {
public:
	void initialize();
	void debug_gui();

public:
	const EnemyMovementsData& get_template(const std::string& name) const;
	const std::unordered_map<std::string, EnemyMovementsData>& template_data() const { return templateData; };

private:
	void reset_select(std::optional<std::string> select_ = std::nullopt);
	void detail_window();

private:
	void load_json_all();
	void export_json_all();

private:
	static const inline std::filesystem::path LoadPath
	{ "./Resources/GameScene/EnemyTemplate/Movements/" };

	std::optional<std::string> select;

	std::unordered_map<std::string, EnemyMovementsData> templateData;
};
