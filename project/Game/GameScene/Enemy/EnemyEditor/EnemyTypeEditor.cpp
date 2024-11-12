#include "EnemyTypeEditor.h"

#define NOMINMAX

#include <fstream>
#include <format>

#include <imgui.h>
#include <Library/Externals/nlohmann/json.hpp>

#include "../BaseEnemy.h"

#include <Engine/Resources/PolygonMesh/PolygonMeshManager.h>

using json = nlohmann::json;

void EnemyTypeEditor::initialize() {
	meshCurrentPath = "./Resources";
	get_file_list(meshFileList, meshCurrentPath, ".obj");
	load_json();
	load_obj();
}

void EnemyTypeEditor::debug_gui() {
	ImGui::Begin("EnemyTypeEditor");
	if (ImGui::Button("Save")) {
		export_json_all();
	}

	static char TemplateName[256]{};
	if (ImGui::Button("Create")) {
		if (!templateData.contains(TemplateName) && TemplateName[0] != '\0') {
			templateData.emplace(TemplateName, EnemyTypeData{});
		}
	}
	ImGui::InputText("TemplateName", TemplateName, 256);

	for (auto& name : templateData | std::ranges::views::keys) {
		if (ImGui::Selectable(name.c_str(), select == name)) {
			reset_select(name);
		}
	}

	ImGui::End();

	detail_window();
	obj_file_list();
}

const EnemyTypeData& EnemyTypeEditor::get_template(const std::string& name) const {
	return templateData.at(name);
}

void EnemyTypeEditor::reset_select(std::optional<std::string> select_) {
	select = select_;
}

void EnemyTypeEditor::detail_window() {
	if (select.has_value()) {
		static ImVec2 WindowPosition;
		static ImVec2 WindowSize;
		bool isOpen = true;
		EnemyTypeData& data = templateData.at(select.value());
		ImGui::SetNextWindowPos(WindowPosition, ImGuiCond_Appearing);
		ImGui::SetNextWindowPos(WindowSize, ImGuiCond_Appearing);
		ImGui::Begin(std::format("Detail {}##Type", select.value()).c_str(), &isOpen);
		ImGui::InputInt("Hitpoint", &data.hitpoint);
		ImGui::DragFloat("Radius", &data.radius, 0.01f, 0.0f, 100.0f);
		ImGui::InputInt("Score", &data.score);
		ImGui::Text(std::format("UseObj : {}", data.useObj).c_str());
		if (ImGui::Button("SearchObjFile")) {
			searchResultAddress = &templateData.at(select.value()).useObj;
		}
		if (searchResultAddress) {
			data.objDirectory = meshCurrentPath.string();
		}
		WindowPosition = ImGui::GetWindowPos();
		WindowSize = ImGui::GetWindowSize();
		ImGui::End();
		if (!isOpen) {
			reset_select(std::nullopt);
		}
	}
}

void EnemyTypeEditor::obj_file_list() {
	if (!searchResultAddress) {
		return;
	}

	bool isOpen = true;
	ImGui::Begin("MeshSearch", &isOpen);
	if (ImGui::Button("<-")) {
		if (meshCurrentPath != ".") {
			meshCurrentPath = meshCurrentPath.parent_path();
			get_file_list(meshFileList, meshCurrentPath, ".obj");
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Reload")) {
		get_file_list(meshFileList, meshCurrentPath, ".obj");
	}
	ImGui::SameLine();
	ImGui::Text(std::format("Current-\'{:}\'", meshCurrentPath.string()).c_str());
	for (int i = 0; i < meshFileList.size(); ++i) {
		if (ImGui::Button(meshFileList[i].c_str())) {
			std::filesystem::path select = meshCurrentPath.string() + "/" + meshFileList[i];
			if (std::filesystem::is_directory(select)) {
				meshCurrentPath = std::move(select);
				get_file_list(meshFileList, meshCurrentPath, ".obj");
			}
			else if (std::filesystem::exists(meshCurrentPath.string() + "/" + meshFileList[i])) {
				//meshCurrentPath.string(), meshFileList[i]
				*searchResultAddress = meshFileList[i];
				searchResultAddress = nullptr;
			}
		}
	}
	ImGui::End();

	if (!isOpen) {
		searchResultAddress = nullptr;
	}
}

void EnemyTypeEditor::get_file_list(std::vector<std::string>& list, const std::filesystem::path& path, const std::string& extension) {
	std::filesystem::directory_iterator folderItr{ path };
	std::filesystem::directory_iterator end;

	list.clear();

	std::vector<std::string> folders;
	while (folderItr != end) {
		if (folderItr->is_directory()) {
			folders.emplace_back(folderItr->path().filename().string());
		}
		++folderItr;
	}

	std::filesystem::directory_iterator fileItr{ path };

	std::vector<std::string> files;
	while (fileItr != end) {
		if (fileItr->path().extension() == extension) {
			files.emplace_back(fileItr->path().filename().string());
		}
		++fileItr;
	}

	std::sort(folders.begin(), folders.end());
	std::sort(files.begin(), files.end());

	list = std::move(folders);
	list.insert(list.end(), files.begin(), files.end());
}

void EnemyTypeEditor::load_json() {
	std::filesystem::path directory{ LoadPath.string() + "enemy_type.json" };
	// open file
	std::ifstream ifstream{ directory };

	// Json読み込み
	json root{ json::parse(ifstream) };
	// 閉じる
	ifstream.close();

	for (auto& data : root.items()) {
		std::string targetName = data.key();

		json& data = root.at(targetName);

		auto& write = templateData.emplace(targetName, EnemyTypeData{}).first->second;
		write.hitpoint = data["Hitpoint"];
		write.radius = data["Radius"];
		write.score = data["Score"];
		write.useObj = data["UseObj"];
		write.objDirectory = data["ObjDirectory"];
	}
}

void EnemyTypeEditor::export_json_all() {
	json root;
	for (auto& data : templateData) {
		auto& name = data.first;
		auto& type = data.second;
		root[name] = json::object();
		json& main = root[name];
		main["Hitpoint"] = type.hitpoint;
		main["Radius"] = type.radius;
		main["Score"] = type.score;
		main["UseObj"] = type.useObj;
		main["objDirectory"] = type.objDirectory;

	}
	std::ofstream ofstream{ LoadPath.string() + "enemy_type.json", std::ios::trunc };
	ofstream << std::setfill('\t') << std::setw(1) << root;
	ofstream.close();
}

void EnemyTypeEditor::load_obj() {
	for (auto& data : templateData | std::ranges::views::values) {
		PolygonMeshManager::RegisterLoadQue(data.objDirectory, data.useObj);
	}
}
