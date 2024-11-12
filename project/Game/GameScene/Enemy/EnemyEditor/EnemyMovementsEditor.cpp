#include "EnemyMovementsEditor.h"

#define NOMINMAX

#include <fstream>
#include <format>

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

#include "../BaseEnemy.h"

#include <Library/Externals/nlohmann/json.hpp>

using json = nlohmann::json;

void EnemyMovementsEditor::initialize() {
	load_json_all();
}

#ifdef _DEBUG
void EnemyMovementsEditor::debug_gui() {
	ImGui::Begin("EnemyMovementsEditor");
	if (ImGui::Button("Save")) {
		export_json_all();
	}

	static char TemplateName[256]{};
	if (ImGui::Button("Create")) {
		if (!templateData.contains(TemplateName) && TemplateName[0] != '\0') {
			templateData.emplace(TemplateName, EnemyMovementsData{});
		}
	}
	ImGui::InputText("TemplateName", TemplateName, 256);

	for (auto & name : templateData | std::ranges::views::keys) {
		if (ImGui::Selectable(name.c_str(), select == name)) {
			reset_select(name);
		}
	}

	ImGui::End();

	detail_window();
}
#endif // _DEBUG

const EnemyMovementsData& EnemyMovementsEditor::get_template(const std::string& name) const {
	return templateData.at(name);
}

void EnemyMovementsEditor::reset_select(std::optional<std::string> select_) {
	select = select_;
}

#ifdef _DEBUG
void EnemyMovementsEditor::detail_window() {
	if (select.has_value()) {
		static ImVec2 WindowPosition;
		static ImVec2 WindowSize;
		bool isOpen = true;
		EnemyMovementsData& data = templateData.at(select.value());
		ImGui::SetNextWindowPos(WindowPosition, ImGuiCond_Appearing);
		ImGui::SetNextWindowPos(WindowSize, ImGuiCond_Appearing);
		ImGui::Begin(std::format("Detail {}##MOVE", select.value()).c_str(), &isOpen);
		ImGui::DragFloat("Time", &data.Time, 0.1f);
		for (int i = 0;  auto & control : data.controls) {
			ImGui::DragFloat3(std::format("ControlPoint{}", i).c_str(), &control.x, 0.01f);
			++i;
		}
		if (ImGui::Button("Create")) {
			data.controls.emplace_back();
		}
		WindowPosition = ImGui::GetWindowPos();
		WindowSize = ImGui::GetWindowSize();
		ImGui::End();
		if (!isOpen) {
			reset_select(std::nullopt);
		}
	}
}
#endif // _DEBUG

void EnemyMovementsEditor::load_json_all() {
	std::filesystem::directory_iterator directoryItr{ LoadPath };
	// open file
	for (auto& directory : directoryItr) {
		std::ifstream ifstream{ directory.path() };

		// Json読み込み
		json root{ json::parse(ifstream) };
		// 閉じる
		ifstream.close();

		std::string targetName = directory.path().stem().string();

		json& data = root.at(targetName);

		auto& write = templateData.emplace(targetName, EnemyMovementsData{}).first->second;
		write.Time = data["Time"];
		json& controls = data["Controls"];
		for (int i = 0; i < controls.size(); ++i) {
			auto& array = controls[std::format("{:02}", i)];
			write.controls.emplace_back(array.at(0), array.at(1), array.at(2));
		}
	}
}

void EnemyMovementsEditor::export_json_all() {
	for (auto& data : templateData) {
		auto& name = data.first;
		auto& movements = data.second;
		json root;
		root[name] = json::object();
		json& main = root[name];
		main["Time"] = movements.Time;
		main["Controls"] = json::object();
		for (int i = 0;  auto & control : movements.controls) {
			main["Controls"][std::format("{:02}", i)] = json::array({ control.x,control.y,control.z });
			++i;
		}

		std::ofstream ofstream{ LoadPath.string() + name + ".json", std::ios::trunc };
		ofstream << std::setfill('\t') << std::setw(1) << root;
		ofstream.close();
	}
}
