#include "Transform3D.h"

#include <cmath>

#ifdef _DEBUG
#include "externals/imgui/imgui.h"
#include <format>
#include "Engine/Math/Definition.h"
#endif // _DEBUG


Transform3D::Transform3D() noexcept :
	Transform3D(CVector3::BASIS, {}, CVector3::ZERO) {
}

Transform3D::Transform3D(const Vector3& scale_, const Quaternion& quaternion, const Vector3& translate_) noexcept :
	scale(scale_),
	rotate(quaternion),
	translate(translate_) {
	isNeedUpdate = true;
}

Transform3D::Transform3D(Vector3&& scale_, Quaternion&& quaternion, Vector3&& translate_) noexcept :
	scale(std::move(scale_)),
	rotate(std::move(quaternion)),
	translate(std::move(translate_)) {
	isNeedUpdate = true;
}

void Transform3D::set_scale(const Vector3& scale_) noexcept {
	if (scale != scale_) {
		scale = scale_;
		isNeedUpdate = true;
	}
}

void Transform3D::set_rotate(const Quaternion& rotate_) noexcept {
	if (rotate != rotate_) {
		rotate = rotate_;
		isNeedUpdate = true;
	}
}

void Transform3D::set_translate(const Vector3& translate_) noexcept {
	if (translate != translate_) {
		translate = translate_;
		isNeedUpdate = true;
	}
}

void Transform3D::set_translate_x(float x) noexcept {
	if (translate.x != x) {
		translate.x = x;
		isNeedUpdate = true;
	}
}

void Transform3D::set_translate_y(float y) noexcept {
	if (translate.y != y) {
		translate.y = y;
		isNeedUpdate = true;
	}
}

void Transform3D::set_translate_z(float z) noexcept {
	if (translate.z != z) {
		translate.z = z;
		isNeedUpdate = true;
	}
}

void Transform3D::begin() noexcept {
	isNeedUpdate = false;
}

Matrix4x4 Transform3D::get_matrix() const noexcept {
	return Transform3D::MakeAffineMatrix(scale, rotate, translate);;
}

const Vector3& Transform3D::get_scale() const noexcept {
	return scale;
}

const Vector3& Transform3D::get_translate() const noexcept {
	return translate;
}

const Quaternion& Transform3D::get_quaternion() const noexcept {
	return rotate;
}

void Transform3D::plus_translate(const Vector3& plus) noexcept {
	if (plus.length() != 0) {
		isNeedUpdate = true;
	}
	translate += plus;
}

bool Transform3D::need_update_matrix() const noexcept {
	return isNeedUpdate;
}

void Transform3D::debug_gui() {
#ifdef _DEBUG
	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (ImGui::TreeNode(std::format("Transform3D({:})", (void*)this).c_str())) {
		if (ImGui::Button("ResetScale")) {
			scale = CVector3::BASIS;
			isNeedUpdate = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("ResetRotate")) {
			rotate = Quaternion{};
			isNeedUpdate = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("ResetTranslate")) {
			translate = CVector3::ZERO;
			isNeedUpdate = true;
		}
		if (ImGui::DragFloat3("Scale", &scale.x, 0.01f)) {
			isNeedUpdate = true;
		}
		Vector3 quaternion = CVector3::ZERO;
		if (ImGui::DragFloat3("RotateLocal", &quaternion.x, 0.01f, -PI, PI)) {
			rotate = Quaternion{ quaternion } *rotate;
			isNeedUpdate = true;
		}
		Vector3 cood = CVector3::ZERO;
		if (ImGui::DragFloat3("RotateWorld", &cood.x, 0.02f)) {
			rotate *= Quaternion{ cood, cood.length() };
			isNeedUpdate = true;
		}
		if (ImGui::DragFloat3("Translate", &translate.x, 0.1f)) {
			isNeedUpdate = true;
		}
		ImGui::TreePop();
	}
#endif // _DEBUG
}

//void Transform3D::debug_axis(const Matrix4x4& debug_matrix) const {
//#ifdef _DEBUG
//	static constexpr float __axisLength = 50;
//	Vector3 initial = Transform3D::Homogeneous(CVector3::kZero, debug_matrix);
//	Vector3 terminalX = Transform3D::Homogeneous(CVector3::kBasisX * __axisLength, debug_matrix);
//	Vector3 terminalY = Transform3D::Homogeneous(CVector3::kBasisY * __axisLength, debug_matrix);
//	Vector3 terminalZ = Transform3D::Homogeneous(CVector3::kBasisZ * __axisLength, debug_matrix);
//
//	Renderer::DrawLine(initial, terminalX, BLUE);
//	Renderer::DrawLine(initial, terminalY, GREEN);
//	Renderer::DrawLine(initial, terminalZ, RED);
//#else
//	debug_matrix;
//#endif // _DEBUG
//}

constexpr Matrix4x4 Transform3D::MakeIdentityMatrix() noexcept {
	return CMatrix4x4::IDENTITY;
}

Matrix4x4 Transform3D::MakeRotateXMatrix(const float theta) noexcept {
	return { {
		{ 1, 0, 0, 0 },
		{ 0, std::cos(theta), std::sin(theta), 0},
		{ 0, -std::sin(theta), std::cos(theta), 0 },
		{ 0, 0, 0, 1 }
		} };
}

Matrix4x4 Transform3D::MakeRotateYMatrix(const float theta) noexcept {
	return { {
		{ std::cos(theta), 0, -std::sin(theta), 0 },
		{ 0, 1, 0, 0},
		{ std::sin(theta), 0, std::cos(theta), 0 },
		{ 0, 0, 0, 1 }
		} };
}

Matrix4x4 Transform3D::MakeRotateZMatrix(const float theta) noexcept {
	return { {
		{ std::cos(theta), std::sin(theta), 0, 0},
		{ -std::sin(theta), std::cos(theta), 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 }
		} };
}

Matrix4x4 Transform3D::MakeRotateMatrix(const float x, const float y, const float z) noexcept {
	return MakeRotateXMatrix(x) * MakeRotateYMatrix(y) * MakeRotateZMatrix(z);
}

Matrix4x4 Transform3D::MakeAffineMatrix(const Vector3& scale, const Quaternion& rotate, const Vector3& translate) noexcept {
	Matrix4x4 result2 = rotate.to_matrix();
	for (size_t column = 0; column < 3; ++column) {
		result2[0][column] *= scale.x;
		result2[1][column] *= scale.y;
		result2[2][column] *= scale.z;
	}
	result2[3][0] = translate.x;
	result2[3][1] = translate.y;
	result2[3][2] = translate.z;
	return result2;
}

Vector3 Transform3D::Homogeneous(const Vector3& vector, const Matrix4x4& matrix) {
	float w = vector.x * matrix[0][3] + vector.y * matrix[1][3] + vector.z * matrix[2][3] + 1.0f * matrix[3][3];
	//assert(w != 0);
	return {
		(vector.x * matrix[0][0] + vector.y * matrix[1][0] + vector.z * matrix[2][0] + 1.0f * matrix[3][0]) / w,
		(vector.x * matrix[0][1] + vector.y * matrix[1][1] + vector.z * matrix[2][1] + 1.0f * matrix[3][1]) / w,
		(vector.x * matrix[0][2] + vector.y * matrix[1][2] + vector.z * matrix[2][2] + 1.0f * matrix[3][2]) / w
	};
}

Vector3 Transform3D::HomogeneousVector(const Vector3& vector, const Matrix4x4& matrix) {
	float w = vector.x * matrix[0][3] + vector.y * matrix[1][3] + vector.z * matrix[2][3] + 1.0f * matrix[3][3];
	assert(w != 0);
	return {
		(vector.x * matrix[0][0] + vector.y * matrix[1][0] + vector.z * matrix[2][0] + 1.0f * 0) / w,
		(vector.x * matrix[0][1] + vector.y * matrix[1][1] + vector.z * matrix[2][1] + 1.0f * 0) / w,
		(vector.x * matrix[0][2] + vector.y * matrix[1][2] + vector.z * matrix[2][2] + 1.0f * 0) / w
	};
}