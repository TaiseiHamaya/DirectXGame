#include "RailCamera.h"

#include <Engine/Application/WorldClock/WorldClock.h>

#include "Game/GameScene/Rail/Rail.h"

void RailCamera::update() {
	mileage += speed * WorldClock::DeltaSeconds();
	if (mileage >= rail->rail_length() - 1) {
		mileage = rail->rail_length() - 1;
	}

	rail->transform_from_mileage(*this, mileage);
	Vector3 upward = offset * transform.get_quaternion();
	transform.plus_translate(upward);
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
