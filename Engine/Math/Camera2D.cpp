#include "Camera2D.h"

#include "Engine/WinApp.h"

#undef near
#undef far

#ifdef _DEBUG
#include <externals/imgui/imgui.h>
#endif // _DEBUG

std::unique_ptr<Camera2D> Camera2D::instance;

void Camera2D::Initialize() {
	instance.reset(new Camera2D{});
	SetCameraTransform(Transform2D{Vec2::kBasis, 0, Vec2::kZero});
	instance->SetNDCInfomation(0, static_cast<float>(WinApp::GetClientWidth()), static_cast<float>(WinApp::GetClientHight()), 0, 0, 1000);
	instance->isUpdateOrthro = true;
	instance->InstanceCameraUpdate();
}

void Camera2D::SetCameraPos(const Vector2& pos) {
	instance->camera.set_translate(pos);
}

void Camera2D::SetCameraTransform(const Transform2D& transform) {
	instance->camera = transform;
}

void Camera2D::SetNDCInfomation(float left, float right, float bottom, float top, float near, float far) {
	instance->ndcLeftBottomNear = { left,bottom, near };
	instance->ndcRightTopFar = { right, top, far };
}

void Camera2D::Begin() {
	instance->isUpdateOrthro = false;
	instance->isUpdateVP = false;
	instance->camera.begin();
}

void Camera2D::CameraUpdate() {
	instance->InstanceCameraUpdate();
}

const Matrix4x4& Camera2D::GetVPMatrix() {
	return instance->vpMatrix;
}

bool Camera2D::IsUpdatedVPMatrix() {
    return instance->isUpdateVP;
}

void Camera2D::InstanceCameraUpdate() {
	MakeViewMatrix();
	MakeOrthoMatrix();
	if (camera.need_update_matrix() || isUpdateOrthro) {
		vpMatrix = viewMatrix * orthoMatrix;
		isUpdateVP = true;
	}
}

void Camera2D::MakeViewMatrix() {
	if (camera.need_update_matrix()) {
		viewMatrix = camera.get_matrix4x4_transform().inverse();
	}
}

void Camera2D::MakeOrthoMatrix() {
	if (isUpdateOrthro) {
		orthoMatrix =
		{ { 2 / (ndcRightTopFar.x - ndcLeftBottomNear.x),0,0,0},
		{0,2 / (ndcRightTopFar.y - ndcLeftBottomNear.y),0,0},
		{0,0, 1 / (ndcRightTopFar.z - ndcLeftBottomNear.z),0},
		{(ndcLeftBottomNear.x + ndcRightTopFar.x) / (ndcLeftBottomNear.x - ndcRightTopFar.x),(ndcLeftBottomNear.y + ndcRightTopFar.y) / (ndcLeftBottomNear.y - ndcRightTopFar.y), ndcLeftBottomNear.z / (ndcLeftBottomNear.z - ndcRightTopFar.z), 1} };
	}
}

#ifdef _DEBUG
void Camera2D::DebugGUI() {
	instance->debug_gui();
}

void Camera2D::debug_gui() {
	ImGui::SetNextWindowSize(ImVec2{ 330,140 }, ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2{ 50, 220 }, ImGuiCond_Once);
	ImGui::Begin("2DCamera", nullptr, ImGuiWindowFlags_NoSavedSettings);
	camera.debug_gui();
	ImGui::End();
}
#endif // _DEBUG
