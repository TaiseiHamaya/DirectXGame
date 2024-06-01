#include "Transform2D.h"

#include <cmath>

#include "Engine/GameObject/Transform3D/Transform3D.h"

#ifdef _DEBUG
#include "externals/imgui/imgui.h"
#include <format>
#endif // _DEBUG

Matrix3x3 Transform2D::MakeIdentityMatrix() {
	return { {
		{ 1, 0, 0 },
		{ 0, 1, 0 },
		{ 0, 0, 1 } } };
}

Matrix3x3 Transform2D::MakeRotateMatrix(const float theta) {
	return MakeRotateMatrix(std::sin(theta), std::cos(theta));
}

Matrix3x3 Transform2D::MakeRotateMatrix(const float sine, const float cosine) {
	return { {
		{ cosine, sine, 0 },
		{ -sine, cosine, 0 },
		{ 0, 0, 1 } } };
}

Matrix3x3 Transform2D::MakeScaleMatrix(const float x, const float y) {
	return { {
		{ x, 0, 0 },
		{ 0, y, 0 },
		{ 0, 0, 1 }
		} };
}

Matrix3x3 Transform2D::MakeScaleMatrix(const Vector2& scale) {
	return MakeScaleMatrix(scale.x, scale.y);
}

Matrix3x3 Transform2D::MakeTranslateMatrix(const float x, const float y) {
	return{ {
		{ 1, 0, 0 },
		{ 0, 1, 0 },
		{ x, y, 1 }
		} };
}

Matrix3x3 Transform2D::MakeTranslateMatrix(const Vector2& translate) {
	return MakeTranslateMatrix(translate.x, translate.y);
}

Matrix3x3 Transform2D::MakeAffineMatrix(const Vector2& scale, const float theta, const Vector2& translate) {
	return { {
		{ scale.x * cosf(theta), scale.x * sinf(theta), 0 },
		{ -scale.y * sinf(theta), scale.y * cosf(theta), 0 },
		{ translate.x, translate.y, 1 }
		} };
}

Matrix3x3 Transform2D::MakeAffineMatrix(const Vector2& scale, const float sinTheta, const float cosTheta, const Vector2& translate) {
	return { {
		{ scale.x * cosTheta, scale.x * sinTheta, 0 },
		{ -scale.y * sinTheta, scale.y * cosTheta, 0 },
		{ translate.x, translate.y, 1 }
		} };
}

Vector2 Transform2D::Homogeneous(const Vector2& vector, const Matrix3x3& matrix) {
	float w = vector.x * matrix[0][2] + vector.y * matrix[1][2] + 1.0f * matrix[2][2];
	assert(w != 0);
	return { (vector.x * matrix[0][0] + vector.y * matrix[1][0] + 1.0f * matrix[2][0]) / w, (vector.x * matrix[0][1] + vector.y * matrix[1][1] + 1.0f * matrix[2][1]) / w };;
}

Vector2 Transform2D::HomogeneousVector(const Vector2& vector, const Matrix3x3& matrix) {
	float w = vector.x * matrix[0][2] + vector.y * matrix[1][2] + 1.0f * matrix[2][2];
	assert(w != 0);
	return { (vector.x * matrix[0][0] + vector.y * matrix[1][0] + 1.0f * 0) / w, (vector.x * matrix[0][1] + vector.y * matrix[1][1] + 1.0f * 0) / w };
}

Transform2D::Transform2D() noexcept {
	scale = { 1, 1 };
	rotate = 0;
	translate = { 0, 0 };
	sinTheta = 0;
	cosTheta = 1;
	isNeedUpdate = true;
}

Transform2D::Transform2D(const Vector2& scale_, float rotate_, const Vector2& translate_) noexcept {
	scale = scale_;
	set_rotate(rotate_);
	translate = translate_;
	isNeedUpdate = true;
}

Transform2D::Transform2D(Vector2&& scale_, float rotate_, Vector2&& translate_) noexcept {
	scale = std::move(scale_);
	set_rotate(rotate_);
	translate = std::move(translate_);
	isNeedUpdate = true;
}

void Transform2D::set_scale(const Vector2& scale_) {
	scale = scale_;
	isNeedUpdate = true;
}

void Transform2D::set_rotate(float rotate_) {
	rotate = rotate_;
	set_rotate(std::sin(rotate), std::cos(rotate));
	isNeedUpdate = true;
}

void Transform2D::set_rotate(float sintheta_, float costheta_) {
	sinTheta = sintheta_;
	cosTheta = costheta_;
#ifdef _DEBUG
	//assert(std::round(std::asin(sinTheta) * 10) / 10 == std::round(std::acos(cosTheta) * 10) / 10);
	rotate = std::asin(sinTheta);
#endif // _DEBUG
	isNeedUpdate = true;
}

void Transform2D::set_translate(const Vector2& translate_) {
	if (translate != translate_) {
		isNeedUpdate = true;
	}
	translate = translate_;
}

void Transform2D::set_translate_x(float x) {
	if (translate.x != x) {
		isNeedUpdate = true;
	}
	translate.x = x;
}

void Transform2D::set_translate_y(float y) {
	if (translate.y != y) {
		isNeedUpdate = true;
	}
	translate.y = y;
}

void Transform2D::begin() {
	isNeedUpdate = false;
}

Matrix3x3 Transform2D::get_matrix() const {
	return Transform2D::MakeAffineMatrix(scale, sinTheta, cosTheta, translate);
}

Matrix4x4 Transform2D::get_matrix4x4_transform() const {
	return Transform3D::MakeAffineMatrix(scale.convert_3d(1.0f), Quaternion{ 0,0,rotate }, translate.convert_3d(0.0f));
}

Matrix4x4 Transform2D::get_matrix4x4_padding() const {
	return Matrix4x4::Convert3x3(get_matrix());
}

const Vector2& Transform2D::get_translate() const {
	return translate;
}

void Transform2D::plus_translate(const Vector2& plus) {
	if (plus.length() != 0) {
		isNeedUpdate = true;
	}
	translate += plus;
}

bool Transform2D::need_update_matrix() const {
	return isNeedUpdate;
}

#ifdef _DEBUG
void Transform2D::debug_gui(float translateMove) {
	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (ImGui::TreeNode(std::format("Transform2D({:})", (void*)this).c_str())) {
		if (ImGui::Button("ResetScale")) {
			scale = Vec2::kBasis;
			isNeedUpdate = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("ResetRotate")) {
			rotate = 0;
			isNeedUpdate = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("ResetTranslate")) {
			translate = Vec2::kZero;
			isNeedUpdate = true;
		}
		if (ImGui::DragFloat2("Scale", &scale.x, 0.01f)) {
			isNeedUpdate = true;
		}
		if (ImGui::DragFloat("Rotate", &rotate, 0.02f)) {
			isNeedUpdate = true;
		}
		if (ImGui::DragFloat2("Translate", &translate.x, translateMove)) {
			isNeedUpdate = true;
		}
		ImGui::TreePop();
	}
}
#endif // _DEBUG
