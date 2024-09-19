#include "Camera3D.h"

#include <cmath>

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

	reset_object("Frustum.obj");

#ifdef _DEBUG
	isVaildDebugCamera = false;
	debugCamera = std::make_unique<GameObject>();
	debugCameraCenter = std::make_unique<GameObject>("CameraAxis.obj");
	debugCameraCenter->begin_rendering(*this);
	debugCamera->set_parent(debugCameraCenter->get_hierarchy());
#endif // _DEBUG
}

void Camera3D::update_matrix() {
#ifdef _DEBUG
	if (isVaildDebugCamera) {
		debugCameraCenter->begin_rendering(*this);
		debugCamera->begin_rendering(*this);
	}
#endif // _DEBUG
	this->begin_rendering(*this);

	// リリース時は通常更新
	make_view_matrix();
	make_perspectivefov_matrix();
	vpMatrix = viewMatrix * perspectiveFovMatrix;

#ifdef _DEBUG
	debugVpMatrix = debugViewMatrix * perspectiveFovMatrix;
	if (isVaildDebugCamera) {
		debugCameraCenter->begin_rendering(*this);
	}
#endif // _DEBUG
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

void Camera3D::make_view_matrix() {
	viewMatrix = world_matrix().inverse();
#ifdef _DEBUG
	if (isVaildDebugCamera)
		debugViewMatrix = debugCamera->world_matrix().inverse();
#endif // _DEBUG
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

#ifdef _DEBUG
void Camera3D::debug_gui() {
	transform->debug_gui();
	ImGui::Separator();
	ImGui::Checkbox("DebugCamera", &isVaildDebugCamera);
	if (isVaildDebugCamera) {
		ImGui::DragFloat("Offset", &offset.z, 0.1f, -10000.0f, 0.0f);
		debugCameraCenter->get_transform().debug_gui();
		debugCamera->get_transform().debug_gui();
	}
	debug_camera();
}

void Camera3D::debug_camera() {
	// ioを取得
	auto& io = ImGui::GetIO();
	// デバッグカメラ時のみ実行
	// ImGUIとマウスが重なっているときは実行しない
	if (isVaildDebugCamera && !io.WantCaptureMouse) {
		// マウスの移動量を取得
		ImVec2 ioMouseDelta = io.MouseDelta;
		Vector2 mouseDelta = { ioMouseDelta.x,ioMouseDelta.y };

		// 注視距離設定
		float wheel = io.MouseWheel;
		offset.z = std::min(offset.z + wheel, 0.0f);

		// 中クリック(回転)
		if (ImGui::IsMouseDown(0)) {
			// 倍率をかけて調整
			Vector2 rotateAngle = mouseDelta / 200;
			Quaternion rotation = debugCamera->get_transform().get_quaternion();
			// 平行成分と垂直成分でQuaternionを生成
			Quaternion holizontal = Quaternion::AngleAxis(CVector3::BASIS_Y, rotateAngle.x);
			Quaternion vertical = Quaternion::AngleAxis(CVector3::BASIS_X, rotateAngle.y);
			// 垂直->元->平行で適用させる
			debugCamera->get_transform().set_rotate(holizontal * rotation * vertical);
		}

		// 右クリック(Translate)
		else if (ImGui::IsMouseDown(2)) {
			// Vector3にし、倍率をかける
			Vector3 move = mouseDelta.convert(0) / 100;
			// X軸は反転させる
			move.x *= -1;
			// デバッグカメラの方向を向かせる
			debugCameraCenter->get_transform().plus_translate(move * debugCamera->get_transform().get_quaternion());
		}
	}
	// 位置更新
	debugCamera->get_transform().set_translate(offset * debugCamera->get_transform().get_quaternion());
}

void Camera3D::debug_draw() const {
	if (isVaildDebugCamera) {
		debugCameraCenter->draw();
		this->draw();
	}
}

const Matrix4x4& Camera3D::vp_matrix_draw() const {
	return isVaildDebugCamera ? debugVpMatrix : vpMatrix;
}

#endif // _DEBUG
