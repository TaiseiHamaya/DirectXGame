#include "Camera3D.h"

#include <cmath>

#include "Library/Math/VectorConverter.h"

#include "Engine/Application/EngineSettings.h"
#include "Engine/Rendering/DirectX/DirectXCommand/DirectXCommand.h"

#ifdef _DEBUG
#include <imgui.h>
#include <Engine/Runtime/Input/Input.h>
#endif // _DEBUG

void Camera3D::initialize() {
	set_perspective_fov_info(
		0.45f,
		EngineSettings::CLIENT_SIZE.x / EngineSettings::CLIENT_SIZE.y,
		0.1f, 1000
	);

#ifdef _DEBUG
	isVaildDebugCamera = false;
	debugCamera = std::make_unique<WorldInstance>();
	debugCameraCenter = std::make_unique<MeshInstance>("CameraAxis.obj");
	//debugCameraCenter->begin_rendering();
	debugCamera->set_parent(*debugCameraCenter);
	frustum = std::make_unique<MeshInstance>("Frustum.obj");
	frustum->set_parent(*this);
#endif // _DEBUG

	update_matrix();
}

void Camera3D::update_matrix() {
	// カメラそのもののMatrix更新
	WorldInstance::update_affine();
#ifdef _DEBUG
	if (isVaildDebugCamera) {
		// デバッグ表示に使用するモデルのWorldMatrixの更新
		debugCameraCenter->begin_rendering();
		debugCamera->update_affine();
		// ViewMatrixの更新
		debugViewAffine = debugCamera->world_affine().inverse_fast();
	}
#endif // _DEBUG

	// カメラ位置をもとにViewMatrixを更新
	make_view_matrix();
	make_perspectivefov_matrix();


#ifdef _DEBUG
	// 外部参照用Matrix
	vpMatrix = viewAffine.to_matrix() * perspectiveFovMatrix;
	// 描画用
	if (isVaildDebugCamera) {
		*vpMatrixBuffer.get_data() = debugViewAffine.to_matrix() * perspectiveFovMatrix;
	}
	else {
		*vpMatrixBuffer.get_data() = vpMatrix;
	}
#else
	// リリースビルド時は参照用と描画用が必ず同じになるのでこの実装
	*vpMatrixBuffer.get_data() = viewAffine.to_matrix() * perspectiveFovMatrix;
#endif // _DEBUG
}

void Camera3D::register_world(uint32_t index) {
	auto& commandList = DirectXCommand::GetCommandList();
	commandList->SetGraphicsRootConstantBufferView(
		index, vpMatrixBuffer.get_resource()->GetGPUVirtualAddress()
	);
}

void Camera3D::set_transform(const Transform3D& transform_) noexcept {
	transform.copy(transform_);
}

void Camera3D::set_perspective_fov_info(float fovY_, float aspectRatio_, float nearClip_, float farClip_) noexcept {
	fovY = fovY_;
	aspectRatio = aspectRatio_;
	nearClip = nearClip_;
	farClip = farClip_;
}

const Matrix4x4& Camera3D::vp_matrix() const {
#ifdef _DEBUG
	return vpMatrix;
#else
	return *vpMatrixBuffer.get_data();
#endif // _DEBUG
}

void Camera3D::make_view_matrix() {
	viewAffine = world_affine().inverse_fast();
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

Matrix4x4 Camera3D::MakeViewportMatrix(const Vector2& origin, const Vector2& size, float minDepth, float maxDepth) {
	return {
		{{ size.x / 2, 0, 0, 0 },
		{ 0, -size.y / 2, 0, 0 },
		{ 0, 0, maxDepth - minDepth, 0 },
		{ origin.x + size.x / 2, origin.y + size.y / 2, minDepth, 1 } }
	};
}

#ifdef _DEBUG

#include <string>
#include <source_location>

using namespace std::literals::string_literals;

void Camera3D::debug_gui() {
	transform.debug_gui();

	//if (ImGui::Button("SaveJson")) {
	//	constexpr const char* fileName = "Camera3D";
	//	JsonResource output{ "WorldInstance/"s + fileName + ".json"};
	//	to_json(output);
	//	output.save();
	//}

	ImGui::Separator();
	ImGui::Checkbox("DebugCamera", &isVaildDebugCamera);
	if (isVaildDebugCamera) {
		ImGui::DragFloat("Offset", &offset.z, 0.1f, -std::numeric_limits<float>::infinity(), 0.0f);
		debugCameraCenter->get_transform().debug_gui();
		debugCamera->get_transform().debug_gui();
	}
	debug_camera();
}

void Camera3D::debug_camera() {
	// デバッグカメラ時のみ実行
	// ImGUIとマウスが重なっているときは実行しない
	if (isVaildDebugCamera && !ImGui::GetIO().WantCaptureMouse) {
		// マウスの移動量を取得
		Vector2 mouseDelta = Input::MouseDelta();

		// 注視距離設定
		float wheel = static_cast<float>(Input::WheelDelta());
		offset.z = std::min(offset.z + wheel, 0.0f);

		// 左クリック(回転)
		if (Input::IsPressMouse(MouseID::Left)) {
			// 倍率をかけて調整
			Vector2 rotateAngle = mouseDelta / 200;
			Quaternion rotation = debugCamera->get_transform().get_quaternion();
			// 平行成分と垂直成分でQuaternionを生成
			Quaternion holizontal = Quaternion::AngleAxis(CVector3::BASIS_Y, rotateAngle.x);
			Quaternion vertical = Quaternion::AngleAxis(CVector3::BASIS_X, rotateAngle.y);
			// 垂直->元->平行で適用させる
			debugCamera->get_transform().set_quaternion(holizontal * rotation * vertical);
		}

		// 中クリック(Translate)
		else if (Input::IsPressMouse(MouseID::Middle)) {
			// Vector3にし、倍率をかける
			Vector3 move = Converter::ToVector3(mouseDelta / 100, 0);
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
		if (offset.z < -0.001) {
			debugCameraCenter->draw();
		}
		frustum->begin_rendering();
		frustum->draw();
	}
}

const Matrix4x4& Camera3D::vp_matrix_debug() const {
	return *vpMatrixBuffer.get_data();
}

#endif // _DEBUG
