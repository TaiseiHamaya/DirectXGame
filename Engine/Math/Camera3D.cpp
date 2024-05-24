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
	instance->camera.set_scale(Vec3::kBasis);
	instance->camera.set_rotate(Quaternion{ 0.7f, 0, 0 });
	instance->camera.set_translate({ 0, 10, -10 });
	instance->SetPerspectiveFovInfomation(0.45f, static_cast<float>(WinApp::GetClientWidth()) / static_cast<float>(WinApp::GetClientHight()), 0.1f, 1000);
	instance->InstanceCameraUpdate();
}

void Camera3D::SetCameraPos(const Vector3& pos) {
	instance->camera.set_translate(pos);
}

void Camera3D::SetCameraTransform(const Transform3D& transform) {
	instance->camera.set_scale(transform.get_scale());
	instance->camera.set_rotate(transform.get_quaternion());
	instance->camera.set_translate(transform.get_translate());
}

void Camera3D::SetPerspectiveFovInfomation(float fovY, float aspectRatio, float nearClip, float farClip) {
	instance->fovY = fovY;
	instance->aspectRatio = aspectRatio;
	instance->nearClip = nearClip;
	instance->farClip = farClip;
}

void Camera3D::Begin() {
	instance->camera.begin();
}

void Camera3D::CameraUpdate() {
	instance->InstanceCameraUpdate();
}

const Matrix4x4& Camera3D::GetVPMatrix() {
	return instance->vpMatrix;
}

const Transform3D& Camera3D::GetCameraTransform() {
	return instance->camera;
}

#ifdef _DEBUG
void Camera3D::DebugGUI() {
	instance->_debug_gui();
}
#endif // _DEBUG

void Camera3D::InstanceCameraUpdate() {
	MakeViewMatrix();
	MakePerspectiveFovMatrix();
	vpMatrix = viewMatrix * persectiveFovMatrix;
}

#ifdef _DEBUG
void Camera3D::_debug_gui() {
	ImGui::SetNextWindowSize(ImVec2{ 330,165 }, ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2{ 50, 50 }, ImGuiCond_Once);
	ImGui::Begin("Camera", nullptr, ImGuiWindowFlags_NoSavedSettings);
	camera.debug_gui();
	ImGui::End();
}
#endif // _DEBUG

void Camera3D::MakeViewMatrix() {
	viewMatrix = camera.get_matrix().inverse();
}

void Camera3D::MakePerspectiveFovMatrix() {
	float cot = 1 / std::tan(fovY / 2);
	persectiveFovMatrix = {
		{{ cot / aspectRatio, 0, 0, 0 },
		{ 0, cot, 0, 0 },
		{ 0, 0, farClip / (farClip - nearClip), 1 },
		{ 0, 0, -nearClip * farClip / (farClip - nearClip), 0 } }
	};
}
