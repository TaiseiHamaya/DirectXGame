#include "Transform2D.h"

#include <cmath>

#include "Engine/Game/Transform3D/Transform3D.h"

#ifdef _DEBUG
#include "externals/imgui/imgui.h"
#include <format>
#endif // _DEBUG

Transform2D::Transform2D() noexcept {
	scale = { 1, 1 };
	rotate = 0;
	translate = { 0, 0 };
}

Transform2D::Transform2D(const Vector2& scale_, float rotate_, const Vector2& translate_) noexcept {
	scale = scale_;
	set_rotate(rotate_);
	translate = translate_;
}

Transform2D::Transform2D(Vector2&& scale_, float rotate_, Vector2&& translate_) noexcept {
	scale = std::move(scale_);
	set_rotate(rotate_);
	translate = std::move(translate_);
}

void Transform2D::set_scale(const Vector2& scale_) noexcept {
	scale = scale_;
}

void Transform2D::set_rotate(float rotate_) noexcept {
	rotate = rotate_;
}

void Transform2D::set_translate(const Vector2& translate_) noexcept {
	translate = translate_;
}

void Transform2D::set_translate_x(float x) noexcept {
	translate.x = x;
}

void Transform2D::set_translate_y(float y) noexcept {
	translate.y = y;
}

Matrix3x3 Transform2D::get_matrix() const noexcept {
	return Transform2D::MakeAffineMatrix(scale, rotate, translate);
}

Matrix4x4 Transform2D::get_matrix4x4_transform() const noexcept {
	return Transform3D::MakeAffineMatrix(scale.convert(1.0f), Quaternion::EulerRadian(0, 0, rotate), translate.convert(0.0f));
}

Matrix4x4 Transform2D::get_matrix4x4_padding() const {
	return Matrix4x4::Convert3x3(get_matrix());
}

const Vector2& Transform2D::get_scale() const noexcept {
	return scale;
}

const float& Transform2D::get_rotate() const noexcept {
	return rotate;
}

const Vector2& Transform2D::get_translate() const noexcept {
	return translate;
}

void Transform2D::plus_translate(const Vector2& plus) noexcept {
	translate += plus;
}

void Transform2D::copy(const Transform2D& copy) noexcept {
	scale = copy.scale;
	rotate = copy.rotate;
	translate = copy.translate;
}

#ifdef _DEBUG
void Transform2D::debug_gui(float translateMove) {
	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (ImGui::TreeNode(std::format("Transform2D##{:}", (void*)this).c_str())) {
		if (ImGui::Button("ResetScale")) {
			scale = CVector2::BASIS;
		}
		ImGui::SameLine();
		if (ImGui::Button("ResetRotate")) {
			rotate = 0;
		}
		ImGui::SameLine();
		if (ImGui::Button("ResetTranslate")) {
			translate = CVector2::ZERO;
		}
		ImGui::DragFloat2("Scale", &scale.x, 0.01f);
		ImGui::DragFloat("Rotate", &rotate, 0.02f);
		ImGui::DragFloat2("Translate", &translate.x, translateMove);
		ImGui::TreePop();
	}
}
#endif // _DEBUG

Matrix3x3 Transform2D::MakeRotateMatrix(const float theta) noexcept {
	return MakeRotateMatrix(std::sin(theta), std::cos(theta));
}

Matrix3x3 Transform2D::MakeAffineMatrix(const Vector2& scale, const float theta, const Vector2& translate) noexcept {
	return MakeAffineMatrix(scale, std::sin(theta), std::cos(theta), translate);
}

constexpr Matrix3x3 Transform2D::MakeAffineMatrix(const Vector2& scale, const float sinTheta, const float cosTheta, const Vector2& translate) noexcept {
	return { {
		{ scale.x * cosTheta, scale.x * sinTheta, 0 },
		{ -scale.y * sinTheta, scale.y * cosTheta, 0 },
		{ translate.x, translate.y, 1 }
		} };
}
