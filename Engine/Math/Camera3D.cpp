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
	instance->camera.set_rotate(Quaternion{ 0, 0, 0 });
	instance->camera.set_translate({ 0, 0, -5 });
	instance->SetNDCInfomation(0, static_cast<float>(WinApp::GetClientWidth()), static_cast<float>(WinApp::GetClientHight()), 0, 0, 1000);
	instance->SetPerspectiveFovInfomation(0.45f, static_cast<float>(WinApp::GetClientWidth()) / static_cast<float>(WinApp::GetClientHight()), 0.1f, 1000);
	instance->SetViewportInformation({ 0, 0 }, { static_cast<float>(WinApp::GetClientWidth()), static_cast<float>(WinApp::GetClientHight()) }, 0, 1);
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

void Camera3D::SetNDCInfomation(float left, float right, float bottom, float top, float near, float far) {
	instance->ndcLeftBottomNear = { left,bottom, near };
	instance->ndcRightTopFar = { right, top, far };
}

void Camera3D::SetPerspectiveFovInfomation(float fovY, float aspectRatio, float nearClip, float farClip) {
	instance->fovY = fovY;
	instance->aspectRatio = aspectRatio;
	instance->nearClip = nearClip;
	instance->farClip = farClip;
}

void Camera3D::SetViewportInformation(const Vector2& origin, const Vector2& size, float minDepth, float maxDepth) {
	instance->viewportOrigin = origin;
	instance->viewportSize = size;
	instance->minDepth = minDepth;
	instance->maxDepth = maxDepth;
}

void Camera3D::Begin() {
	instance->camera.begin();
}

void Camera3D::CameraUpdate() {
	instance->InstanceCameraUpdate();
}

const Matrix4x4& Camera3D::GetVPOVMatrix() {
	return instance->vpovMatrix;
}

const Matrix4x4& Camera3D::GetVPMatrix() {
	return instance->vpMatrix;
}

const Matrix4x4& Camera3D::GetOrthroMatrix() {
	return instance->orthoMatrix;
}

const Transform3D& Camera3D::GetCameraTransform() {
	return instance->camera;
}

#ifdef _DEBUG
void Camera3D::DebugGUI() {
	instance->_DebugGUI();
}
#endif // _DEBUG

void Camera3D::InstanceCameraUpdate() {
	MakeViewMatrix();
	MakePersectiveFovMatrix();
	MakeOrthoMatrix();
	MakeViewportMatrix();
	vpMatrix = viewMatrix * persectiveFovMatrix;
	voMatrix = viewMatrix * orthoMatrix;
	vpovMatrix = vpMatrix * viewportMatrix;
}

#ifdef _DEBUG
void Camera3D::_DebugGUI() {
	ImGui::SetNextWindowSize(ImVec2{ 330,140 }, ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2{ 50, 50 }, ImGuiCond_Once);
	ImGui::Begin("Camera", nullptr, ImGuiWindowFlags_NoSavedSettings);
	camera.debug_gui();
	ImGui::End();
}
#endif // _DEBUG

void Camera3D::MakeViewMatrix() {
	viewMatrix = camera.get_matrix().inverse();
}

void Camera3D::MakeOrthoMatrix() {
	orthoMatrix = {
		{{ 2 / (ndcRightTopFar.x - ndcLeftBottomNear.x),0,0,0},
		{0,2 / (ndcRightTopFar.y - ndcLeftBottomNear.y),0,0},
		{0,0, 1 / (ndcRightTopFar.z - ndcLeftBottomNear.z),0},
		{(ndcLeftBottomNear.x + ndcRightTopFar.x) / (ndcLeftBottomNear.x - ndcRightTopFar.x),(ndcLeftBottomNear.y + ndcRightTopFar.y) / (ndcLeftBottomNear.y - ndcRightTopFar.y), ndcLeftBottomNear.z / (ndcLeftBottomNear.z - ndcRightTopFar.z), 1}}

	};
}

void Camera3D::MakePersectiveFovMatrix() {
	float cot = 1 / std::tan(fovY / 2);
	persectiveFovMatrix = {
		{{ cot / aspectRatio, 0, 0, 0 },
		{ 0, cot, 0, 0 },
		{ 0, 0, farClip / (farClip - nearClip), 1 },
		{ 0, 0, -nearClip * farClip / (farClip - nearClip), 0 } }
	};
}

void Camera3D::MakeViewportMatrix() {
	viewportMatrix = {
		{{ viewportSize.x / 2, 0, 0, 0 },
		{ 0, -viewportSize.y / 2, 0, 0 },
		{ 0, 0, maxDepth - minDepth, 0 },
		{ viewportOrigin.x + viewportSize.x / 2, viewportOrigin.y + viewportSize.y / 2, minDepth, 1 } }
	};
}
