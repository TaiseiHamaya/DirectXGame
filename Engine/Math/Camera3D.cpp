#include "Camera3D.h"

#include <cmath>

#define NOMINMAX

#include "Engine/WinApp.h"

#ifdef _DEBUG
#include <externals/imgui/imgui.h>
#endif // _DEBUG

void Camera3D::initialize() {
	set_perspective_fov_info(
		0.45f,
		static_cast<float>(WinApp::GetClientWidth()) / static_cast<float>(WinApp::GetClientHight()),
		0.1f, 1000
	);
}

void Camera3D::update_matrix() {
	// リリース時は通常更新
	make_view_matrix();
	make_perspectivefov_matrix();
	vpMatrix = viewMatrix * perspectiveFovMatrix;
}

void Camera3D::set_transform(const Transform3D& transform_) noexcept {
	transform->copy(transform_);
}

void Camera3D::set_perspective_fov_info(float fovY_, float aspectRatio_, float nearClip_, float farClip_) noexcept {
	fovY = fovY_;
	aspectRatio = aspectRatio_;
	nearClip = nearClip_;
	farClip = farClip_;
}

const Matrix4x4& Camera3D::vp_matrix() const {
	return vpMatrix;
}

#ifdef _DEBUG
void Camera3D::debug_gui() {
	transform->debug_gui();
}
#endif // _DEBUG

void Camera3D::make_view_matrix() {
	viewMatrix = transform->get_matrix().inverse();
}

void Camera3D::make_perspectivefov_matrix() {
	float cot = 1 / std::tan(fovY / 2);
	perspectiveFovMatrix = {
		{{ cot / aspectRatio, 0, 0, 0 },
		{ 0, cot, 0, 0 },
		{ 0, 0, farClip / (farClip - nearClip), 1 },
		{ 0, 0, -nearClip * farClip / (farClip - nearClip), 0 } }
	};
}

