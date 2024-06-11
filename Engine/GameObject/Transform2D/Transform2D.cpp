#include "Transform2D.h"

#include <cmath>

#include "Engine/GameObject/Transform3D/Transform3D.h"

#ifdef _DEBUG
#include "externals/imgui/imgui.h"
#include <format>
#endif // _DEBUG

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

Transform2D& Transform2D::operator=(const Transform2D& opr) noexcept {
	if (scale != opr.scale ||
		translate != opr.translate ||
		sinTheta != opr.sinTheta ||
		cosTheta != opr.cosTheta
		) {
		isNeedUpdate = true;
	}
	scale = opr.scale;
	translate = opr.translate;
	sinTheta = opr.sinTheta;
	cosTheta = opr.cosTheta;
	rotate = opr.rotate;
	return *this;
}

Transform2D& Transform2D::operator=(Transform2D&& opr) noexcept {
	if (scale != opr.scale ||
		translate != opr.translate ||
		sinTheta != opr.sinTheta ||
		cosTheta != opr.cosTheta
		) {
		isNeedUpdate = true;
	}
	scale = std::move(opr.scale);
	translate = std::move(opr.translate);
	sinTheta = std::move(opr.sinTheta);
	cosTheta = std::move(opr.cosTheta);
	rotate = std::move(opr.rotate);
	return *this;
}

void Transform2D::set_scale(const Vector2& scale_) noexcept {
	scale = scale_;
	isNeedUpdate = true;
}

void Transform2D::set_rotate(float rotate_) noexcept {
	rotate = rotate_;
	set_rotate(std::sin(rotate), std::cos(rotate));
	isNeedUpdate = true;
}

void Transform2D::set_rotate(float sintheta_, float costheta_) noexcept {
	sinTheta = sintheta_;
	cosTheta = costheta_;
#ifdef _DEBUG
	//assert(std::round(std::asin(sinTheta) * 10) / 10 == std::round(std::acos(cosTheta) * 10) / 10);
	rotate = std::asin(sinTheta);
#endif // _DEBUG
	isNeedUpdate = true;
}

void Transform2D::set_translate(const Vector2& translate_) noexcept {
	if (translate != translate_) {
		isNeedUpdate = true;
	}
	translate = translate_;
}

void Transform2D::set_translate_x(float x) noexcept {
	if (translate.x != x) {
		isNeedUpdate = true;
	}
	translate.x = x;
}

void Transform2D::set_translate_y(float y) noexcept {
	if (translate.y != y) {
		isNeedUpdate = true;
	}
	translate.y = y;
}

void Transform2D::begin() noexcept {
	isNeedUpdate = false;
}

Matrix3x3 Transform2D::get_matrix() const noexcept {
	return Transform2D::MakeAffineMatrix(scale, sinTheta, cosTheta, translate);
}

Matrix4x4 Transform2D::get_matrix4x4_transform() const noexcept {
	return Transform3D::MakeAffineMatrix(scale.convert(1.0f), Quaternion{ 0,0,rotate }, translate.convert(0.0f));
}

Matrix4x4 Transform2D::get_matrix4x4_padding() const {
	return Matrix4x4::Convert3x3(get_matrix());
}

const Vector2& Transform2D::get_translate() const noexcept {
	return translate;
}

void Transform2D::plus_translate(const Vector2& plus) noexcept {
	if (plus.length() != 0) {
		isNeedUpdate = true;
	}
	translate += plus;
}

bool Transform2D::need_update_matrix() const noexcept {
	return isNeedUpdate;
}

#ifdef _DEBUG
void Transform2D::debug_gui(float translateMove) {
	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (ImGui::TreeNode(std::format("Transform2D({:})", (void*)this).c_str())) {
		if (ImGui::Button("ResetScale")) {
			scale = CVector2::BASIS;
			isNeedUpdate = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("ResetRotate")) {
			rotate = 0;
			isNeedUpdate = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("ResetTranslate")) {
			translate = CVector2::ZERO;
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
