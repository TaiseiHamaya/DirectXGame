#include "RailCamera.h"

#include <algorithm>

#include <Engine/Runtime/WorldClock/WorldClock.h>

#include "Game/GameScene/Rail/Rail.h"

void RailCamera::update() {
	mileage += speed * WorldClock::DeltaSeconds();
	mileage = std::clamp(mileage, 0.0f, rail->rail_length() - 1);

	rail->transform_from_mileage(*this, mileage);
	basePoint = transform.get_translate();
	Vector3 upward = offset * transform.get_quaternion();
	transform.plus_translate(upward);
}

Vector3 RailCamera::camera_upward_point(const Vector3& offset_) const {
	Vector3 upward = offset_ * transform.get_quaternion();
	return basePoint + offset;
}

#ifdef _DEBUG

#include <imgui.h>
void RailCamera::debug_gui() {
	ImGui::Begin("RailCamera");
	
	ImGui::DragFloat("Mileage", &mileage, 0.01f, 0.0f, rail->rail_length());
	ImGui::DragFloat("Speed", &speed, 0.01f);
	ImGui::DragFloat3("Offset", &offset.x, 0.01f);

	Camera3D::debug_gui();
	ImGui::End();
}
#endif // _DEBUG
