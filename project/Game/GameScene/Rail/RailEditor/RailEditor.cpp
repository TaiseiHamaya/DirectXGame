#include "RailEditor.h"
#ifdef _DEBUG
#define NOMINMAX

#include <fstream>
#include <format>

#include "../Rail.h"

#include <imgui.h>
#include <Library/Externals/nlohmann/json.hpp>

using json = nlohmann::json;

void RailEditor::initialize(Rail* rail_) {
	rail = rail_;
	editRailPoints.resize(rail->railPoints.size());
	for (int i = 0; auto& edit : editRailPoints) {
		edit.position = rail->railPoints[i].position;
		edit.upwardAngle = rail->railPoints[i].upwardAngle;
		edit.minSpeed = rail->railPoints[i].minSpeed;
		edit.maxSpeed = rail->railPoints[i].maxSpeed;
		++i;
	}
}

void RailEditor::debug_gui() {
	ImGui::Begin("RailEditor");
	if (ImGui::Button("Save")) {
		export_json();
	}

	if (ImGui::Button("Apply")) {
		apply();
	}

	for (int i = 0; auto & railPoint : editRailPoints) {
		if (ImGui::TreeNode(std::format("Point{}", i).c_str())) {
			ImGui::DragFloat3("Position", &railPoint.position.x, 0.1f);
			if (railPoint.upwardAngle.has_value()) {
				ImGui::SetNextItemWidth(100);
				ImGui::DragFloat("Angle", &railPoint.upwardAngle.value(), 0.01f);
				ImGui::SameLine();
				if (ImGui::Button("Disable")) {
					railPoint.upwardAngle = std::nullopt;
				}
			}
			else {
				if (ImGui::Button("Enable")) {
					railPoint.upwardAngle = 0.0f;
				}
			}
			ImGui::DragFloatRange2("SpeedRange", &railPoint.minSpeed, &railPoint.maxSpeed, 0.01f, 0.0f, 100.0f);
			ImGui::TreePop();
		}
		++i;
	}

	if (ImGui::Button("PushBack")) {
		editRailPoints.emplace_back();
	}

	if (ImGui::Button("PopBack")) {
		editRailPoints.pop_back();
	}

	ImGui::End();
}

void RailEditor::debug_draw() {
	int diff = int(editRailPoints.size()) - int(editDraw.size());
	if (diff) {
		editDraw.resize(editRailPoints.size());
		if (diff > 0) {
			for (int i = 0; auto & edit : editDraw | std::views::reverse) {
				if (diff == i) {
					break;
				}
				edit.reset_object("RailPoint.obj");
				++i;
			}
		}
	}
	for (int i = 0; auto & edit : editRailPoints) {
		editDraw[i].get_transform().set_translate(edit.position);
		editDraw[i].begin_rendering();
		editDraw[i].draw();
		++i;
	}
}

void RailEditor::apply() {
	rail->railPoints.clear();
	for (auto& railPoint : editRailPoints) {
		rail->create_rail_point(
			railPoint.position, railPoint.minSpeed, railPoint.maxSpeed, railPoint.upwardAngle
		);
	}
	rail->apply_rail_point();
	rail->create_rail();
}

void RailEditor::export_json() {
	json root;
	root["Stage"] = json::object();
	auto& array = root["Stage"];
	for (int i = 0;  auto & point : rail->railPoints) {
		std::string name = std::format("{:02}", i);
		json& pointData = array[name];

		pointData["Position"] = json::array({ point.position.x,point.position.y,point.position.z });
		pointData["MinSpeed"] = point.minSpeed;
		pointData["MaxSpeed"] = point.maxSpeed;
		if (point.upwardAngle.has_value()) {
			pointData["Angle"] = point.upwardAngle.value();
		}
		else {
			pointData["Angle"] = nullptr;
		}
		++i;
	}
	std::ofstream ofstream{ Rail::LoadPath.string() + "stage.json", std::ios::trunc };
	ofstream << std::setfill('\t') << std::setw(1) << root;
	ofstream.close();
}
#endif // _DEBUG
