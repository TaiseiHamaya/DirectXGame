#include "Camera2D.h"

#include "Engine/Application/EngineSettings.h"

#undef near
#undef far

#ifdef DEBUG_FEATURES_ENABLE
#include <imgui.h>
#endif // _DEBUG

std::unique_ptr<Camera2D> Camera2D::instance;

void Camera2D::Initialize() {
	instance.reset(new Camera2D{});
	SetCameraTransform(Transform2D{ CVector2::BASIS, 0, CVector2::ZERO });
	instance->SetNDCInformation(0, EngineSettings::CLIENT_SIZE.x, 0, EngineSettings::CLIENT_SIZE.y, 0, 1000);
	instance->camera_update();
}

void Camera2D::SetCameraPos(const Vector2& pos) noexcept {
	instance->camera.set_translate(pos);
}

void Camera2D::SetCameraTransform(const Transform2D& transform) noexcept {
	instance->camera.copy(transform);
}

void Camera2D::SetNDCInformation(r32 left, r32 right, r32 bottom, r32 top, r32 near, r32 far) noexcept {
	instance->ndcLeftBottomNear = { left,bottom, near };
	instance->ndcRightTopFar = { right, top, far };
}

void Camera2D::CameraUpdate() {
	instance->camera_update();
}

const Matrix4x4& Camera2D::GetVPMatrix() noexcept {
	return instance->vpMatrix;
}

void Camera2D::camera_update() {
	make_view_matrix();
	make_ortho_matrix();
	vpMatrix = viewMatrix * orthoMatrix;
}

void Camera2D::make_view_matrix() {
	viewMatrix = camera.get_matrix4x4_transform().inverse();
}

void Camera2D::make_ortho_matrix() {
	orthoMatrix =
	{ { 2 / (ndcRightTopFar.x - ndcLeftBottomNear.x),0,0,0},
	{0,2 / (ndcRightTopFar.y - ndcLeftBottomNear.y),0,0},
	{0,0, 1 / (ndcRightTopFar.z - ndcLeftBottomNear.z),0},
	{(ndcLeftBottomNear.x + ndcRightTopFar.x) / (ndcLeftBottomNear.x - ndcRightTopFar.x),(ndcLeftBottomNear.y + ndcRightTopFar.y) / (ndcLeftBottomNear.y - ndcRightTopFar.y), ndcLeftBottomNear.z / (ndcLeftBottomNear.z - ndcRightTopFar.z), 1} };
}

#ifdef DEBUG_FEATURES_ENABLE
void Camera2D::DebugGUI() {
	instance->debug_gui();
}

void Camera2D::debug_gui(string_literal tag) {
	//ImGui::SetNextWindowSize(ImVec2{ 330,140 }, ImGuiCond_Once);
	//ImGui::SetNextWindowPos(ImVec2{ 20, 20 }, ImGuiCond_Once);
	ImGui::Begin(tag, nullptr);
	camera.debug_gui();
	ImGui::End();
}
#endif // _DEBUG
