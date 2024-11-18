#include "Timeline.h"

#define NOMINMAX

#include <fstream>

#include <Windows.h>
#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG
#include <Library/Externals/nlohmann/json.hpp>

#include <Engine/Debug/Output.h>
#include <Engine/Runtime/WorldClock/WorldClock.h>

#include "Game/GameScene/Enemy/EnemyManager.h"
#include "Game/GameScene/Player/RailCamera.h"

using json = nlohmann::json;

void Timeline::initialize(EnemyManager* enemyManager_, RailCamera* railCamera_) {
	enemyManager = enemyManager_;
	railCamera = railCamera_;
	load_pop_data();

#ifdef _DEBUG
	initialize_editor(enemyManager->get_type_database(), enemyManager->get_movement_database());
	editData = data;
#endif // _DEBUG
}

void Timeline::update() {

	auto next = data.begin();

	while (data.end() != next && railCamera->get_mileage() >= next->mileage) {
		enemyManager->create(next->translate, next->type, next->movement);
		data.pop_front();
		next = data.begin();
	}
}

void Timeline::load_pop_data() {
	data.clear();

	std::filesystem::path directory{ LoadPath.string() + "timeline.json" };
	std::ifstream ifstream{ directory };

	// 開けなかったらログを出す
	if (ifstream.fail()) {
		std::string message = std::format("[Rail] Failed open stage file. \'{}\'\n", directory.string());
		ConsoleA(message);
		MessageBoxA(nullptr, message.c_str(), "Rail", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	// Json読み込み
	json root{ json::parse(ifstream) };
	// 閉じる
	ifstream.close();

	for (const auto& read : root) {
		data.emplace_back(
			read["Mileage"],
			Vector3{ read["Translate"].at(0), read["Translate"].at(1),read["Translate"].at(2) },
			read["Type"],
			read["Movement"]
		);
	}
}

#ifdef _DEBUG
void Timeline::initialize_editor(const EnemyTypeEditor* typeDatabase_, const EnemyMovementsEditor* moveDatabase_) {
	typeDatabase = typeDatabase_;
	moveDatabase = moveDatabase_;
}

void Timeline::debug_gui() {
	ImGui::Begin("Timeline");

	if (ImGui::Button("Export")) {
		export_pop_data();
	}
	if (ImGui::Button("Restart")) {
		load_pop_data();
	}
	if (ImGui::Button("Apply")) {
		data = editData;
	}

	for (int i = 0; auto & data : editData) {
		if (ImGui::Selectable(std::format("{:02}", i).c_str(), detail == &data)) {
			detail = &data;
		}
		++i;
	}

	if (ImGui::Button("Create")) {
		editData.emplace_back();
	}

	ImGui::End();

	detail_window();
}

void Timeline::detail_window() {
	if (!detail) {
		return;
	}

	bool isOpen = true;
	ImGui::Begin("DetailWindow", &isOpen);
	ImGui::DragFloat("Mileage", &detail->mileage, 0.01f, 10.0f, std::numeric_limits<float>::infinity());
	ImGui::DragFloat3("Translate", &detail->translate.x, 0.01f);
	if (ImGui::BeginCombo("Type", detail->type.c_str())) {
		auto& templates = typeDatabase->template_data();
		for (auto& data : templates | std::ranges::views::keys) {
			if (ImGui::Selectable(data.c_str(), data == detail->type)) {
				detail->type = data;
			}
			if (data == detail->type) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	if (ImGui::BeginCombo("Movement", detail->movement.c_str())) {
		auto& templates = moveDatabase->template_data();
		for (auto& data : templates | std::ranges::views::keys) {
			if (ImGui::Selectable(data.c_str(), data == detail->movement)) {
				detail->movement = data;
			}
			if (data == detail->movement) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	ImGui::End();

	if (!isOpen) {
		detail = nullptr;
	}
}

void Timeline::export_pop_data() {
	json root;
	for (int i = 0; auto & popData : editData) {
		std::string name = std::format("{:02}", i);
		json& popJson = root[name];
		popJson["Mileage"] = popData.mileage;
		popJson["Translate"] = json::array({ popData.translate.x,popData.translate.y,popData.translate.z });
		popJson["Type"] = popData.type;
		popJson["Movement"] = popData.movement;
		++i;
	}
	std::ofstream ofstream{ LoadPath.string() + "timeline.json", std::ios::trunc };
	ofstream << std::setfill('\t') << std::setw(1) << root;
	ofstream.close();
}
#endif // _DEBUG
