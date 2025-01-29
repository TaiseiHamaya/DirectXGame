#include "PointLightInstance.h"

void PointLightInstance::update_affine() {
	float scale = lightData.radius + 0.1f;
	transform.set_scale(
		{ scale, scale, scale }
	);
	WorldInstance::update_affine();
}

void PointLightInstance::transfer() {
	lightData.position = world_position();
}

Matrix4x4 PointLightInstance::transform_matrix() const {
	return world_affine().to_matrix();
}

#ifdef _DEBUG
#include <imgui.h>
void PointLightInstance::debug_gui() {
	// Transform
	transform.debug_gui();
	// 色
	lightData.color.debug_gui();

	constexpr float FLOAT_MAX = (std::numeric_limits<float>::max)(); // 定数
	// 輝度
	ImGui::DragFloat("Intensity", &lightData.intensity, 0.01f, 0.0f, FLOAT_MAX);
	// 範囲
	ImGui::DragFloat("Radius", &lightData.radius, 0.01f, 0.0f, FLOAT_MAX);
	// 範囲
	ImGui::DragFloat("Decay", &lightData.decay, 0.01f, 0.0f, FLOAT_MAX);
}
#endif // _DEBUG
