#include "SpotLightInstance.h"

void SpotLightInstance::begin_rendering() {
	update_affine();

	auto lightData = lightBuffer.get_data();
	lightData->position = world_position();
}

#ifdef _DEBUG
#include <imgui.h>
#include <format>
void SpotLightInstance::debug_gui() {
	auto lightData = lightBuffer.get_data();

	if (!lightData) {
		return;
	}

	// Transform
	transform.debug_gui();
	// 色
	lightData->color.debug_gui();

	constexpr float FLOAT_MAX = (std::numeric_limits<float>::max)(); // 定数
	// 輝度
	ImGui::DragFloat("Intensity", &lightData->intensity, 0.01f, 0.0f, FLOAT_MAX);
	// 距離
	ImGui::DragFloat("Distance", &lightData->distance, 0.01f, 0.0f, FLOAT_MAX);
	// 距離減衰率
	ImGui::DragFloat("DistanceDecay", &lightData->decay, 0.01f, 0.0f, FLOAT_MAX);
	// ライト角
	ImGui::DragFloat("CosAngle", &lightData->angle, 0.01f, -1.0f, std::min(1.0f, lightData->falloffStart));
	// 減衰開始角
	ImGui::DragFloat("FalloffStart", &lightData->falloffStart, 0.01f, lightData->angle, FLOAT_MAX);
	// 向き
	Vector3 rotate = CVector3::ZERO;
	ImGui::Text(std::format("X : {:.3}, Y : {:.3}, Z : {:.3}", lightData->direction.x, lightData->direction.y, lightData->direction.z).c_str());
	if (ImGui::DragFloat3("DirectionRotate", &rotate.x, 1.0f, -180.0f, 180.0f)) {
		lightData->direction = lightData->direction * Quaternion::EulerDegree(rotate);
	}
	// 向きリセットボタン
	ImGui::Text("ResetDirection");
	if (ImGui::Button("X")) {
		lightData->direction = CVector3::BASIS_X;
	}
	ImGui::SameLine();
	if (ImGui::Button("Y")) {
		lightData->direction = CVector3::BASIS_Y;
	}
	ImGui::SameLine();
	if (ImGui::Button("Z")) {
		lightData->direction = CVector3::BASIS_Z;
	}
	ImGui::SameLine();
	if (ImGui::Button("-X")) {
		lightData->direction = -CVector3::BASIS_X;
	}
	ImGui::SameLine();
	if (ImGui::Button("-Y")) {
		lightData->direction = -CVector3::BASIS_Y;
	}
	ImGui::SameLine();
	if (ImGui::Button("-Z")) {
		lightData->direction = -CVector3::BASIS_Z;
	}
}
#endif // _DEBUG
