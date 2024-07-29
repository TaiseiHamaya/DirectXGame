#include "Camera3D.h"

#include <cmath>

#include "Engine/WinApp.h"
#undef near
#undef far

#ifdef _DEBUG
#include <externals/imgui/imgui.h>
#endif // _DEBUG


std::unique_ptr<Camera3D> Camera3D::instance;

void Camera3D::Initialize() {
	instance.reset(new Camera3D{});
	instance->camera.set_scale(CVector3::BASIS);
	instance->camera.set_rotate(Quaternion::EulerDegree(45, 0, 0));
	instance->camera.set_translate({ 0, 10, -10 });
	instance->SetPerspectiveFovInfomation(0.45f, static_cast<float>(WinApp::GetClientWidth()) / static_cast<float>(WinApp::GetClientHight()), 0.1f, 1000);
	instance->camera_update();
}

void Camera3D::SetCameraPos(const Vector3& pos) noexcept {
	instance->camera.set_translate(pos);
}

void Camera3D::SetCameraTransform(const Transform3D& transform) noexcept {
	instance->camera.copy(transform);
}

void Camera3D::SetPerspectiveFovInfomation(float fovY, float aspectRatio, float nearClip, float farClip) noexcept {
	instance->fovY = fovY;
	instance->aspectRatio = aspectRatio;
	instance->nearClip = nearClip;
	instance->farClip = farClip;
}

void Camera3D::CameraUpdate() {
	instance->camera_update();
}

const Matrix4x4& Camera3D::GetVPMatrix() noexcept {
	return instance->vpMatrix;
}

const Transform3D& Camera3D::GetCameraTransform() noexcept {
	return instance->camera;
}

#ifdef _DEBUG
void Camera3D::DebugGUI() {
	instance->_debug_gui();
}
#endif // _DEBUG

void Camera3D::camera_update() {
	make_view_matrix();
	make_perspectivefov_matrix();
	vpMatrix = viewMatrix * persectiveFovMatrix;
}

#ifdef _DEBUG
void Camera3D::_debug_gui() {
	ImGui::SetNextWindowSize(ImVec2{ 330,165 }, ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2{ 20, 20 }, ImGuiCond_Once);
	ImGui::Begin("3DCamera", nullptr, ImGuiWindowFlags_NoSavedSettings);
	camera.debug_gui();
	ImGui::End();
}
#endif // _DEBUG

void Camera3D::make_view_matrix() {
	viewMatrix = camera.get_matrix().inverse();
}

void Camera3D::make_perspectivefov_matrix() {
	float cot = 1 / std::tan(fovY / 2);
	persectiveFovMatrix = {
		{{ cot / aspectRatio, 0, 0, 0 },
		{ 0, cot, 0, 0 },
		{ 0, 0, farClip / (farClip - nearClip), 1 },
		{ 0, 0, -nearClip * farClip / (farClip - nearClip), 0 } }
	};
}
