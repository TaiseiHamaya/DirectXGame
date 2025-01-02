#include "PointLightInstance.h"

void PointLightInstance::begin_rendering() {
	update_affine();

	auto lightData = lightBuffer.get_data();
	lightData->position = world_position();
}

#ifdef _DEBUG
#include <imgui.h>
void PointLightInstance::debug_gui() {
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
	// 範囲
	ImGui::DragFloat("Radius", &lightData->radius, 0.01f, 0.0f, FLOAT_MAX);
	// 範囲
	ImGui::DragFloat("Decay", &lightData->decay, 0.01f, 0.0f, FLOAT_MAX);
}
#endif // _DEBUG
