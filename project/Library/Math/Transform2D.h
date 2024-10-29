#pragma once

#include "Library/Math/Matrix3x3.h"
#include "Library/Math/Matrix4x4.h"
#include "Library/Math/Vector2.h"

class Transform2D final {
public:
	Transform2D() noexcept;
	Transform2D(const Vector2& scale_, float rotate_, const Vector2& translate_) noexcept;
	Transform2D(Vector2&& scale_, float rotate_, Vector2&& translate_) noexcept;
	~Transform2D() noexcept = default;

public:
	Transform2D(Transform2D&&) noexcept = default;
	Transform2D& operator=(Transform2D&&) noexcept = default;

public:
	void set_scale(const Vector2& scale_) noexcept;
	void set_rotate(float rotate_) noexcept;
	void set_translate(const Vector2& translate_) noexcept;
	void set_translate_x(float x) noexcept;
	void set_translate_y(float y) noexcept;
	Matrix3x3 get_matrix() const noexcept;
	Matrix4x4 get_matrix4x4_transform() const noexcept;
	Matrix4x4 get_matrix4x4_padding() const;
	const Vector2& get_scale() const noexcept;
	const float& get_rotate() const noexcept;
	const Vector2& get_translate() const noexcept;
	void plus_translate(const Vector2& plus)noexcept;

	void copy(const Transform2D& copy) noexcept;

#ifdef _DEBUG
public:
	void debug_gui(float translateMove = 0.01f);
#endif // _DEBUG

private:
	Vector2 scale;
	float rotate;
	Vector2 translate;

public:// static関数
	/// <summary>
	/// 単位行列を作成
	/// </summary>
	/// <returns></returns>
	static constexpr Matrix3x3 MakeIdentityMatrix() noexcept;

	/// <summary>
	/// 回転行列を作成
	/// </summary>
	/// <param name="theta">回転</param>
	/// <returns></returns>
	static Matrix3x3 MakeRotateMatrix(const float theta) noexcept;

	/// <summary>
	/// 回転行列を作成
	/// </summary>
	/// <param name="sinTheta">回転</param>
	/// <param name="cosTheta">回転</param>
	/// <returns></returns>
	static constexpr Matrix3x3 MakeRotateMatrix(const float sine, const float cosine) noexcept;

	/// <summary>
	/// 拡縮行列を作成
	/// </summary>
	/// <param name="x">拡縮X</param>
	/// <param name="y">拡縮Y</param>
	/// <returns></returns>
	static constexpr Matrix3x3 MakeScaleMatrix(const float x, const float y) noexcept;

	/// <summary>
	/// 拡縮行列を作成
	/// </summary>
	/// <param name="scale">拡縮</param>
	/// <returns></returns>
	static constexpr Matrix3x3 MakeScaleMatrix(const Vector2& scale) noexcept;

	/// <summary>
	/// 平行移動行列を作成
	/// </summary>
	/// <param name="x">移動X</param>
	/// <param name="y">移動Y</param>
	/// <returns></returns>
	static constexpr Matrix3x3 MakeTranslateMatrix(const float x, const float y) noexcept;

	/// <summary>
	/// 平行移動行列を作成
	/// </summary>
	/// <param name="translate">移動</param>
	/// <returns></returns>
	static constexpr Matrix3x3 MakeTranslateMatrix(const Vector2& translate) noexcept;

	/// <summary>
	/// アフィン変換行列を作成
	/// </summary>
	/// <param name="scale">拡縮</param>
	/// <param name="theta">回転</param>
	/// <param name="translate">移動</param>
	/// <returns></returns>
	static Matrix3x3 MakeAffineMatrix(const Vector2& scale, const float theta, const Vector2& translate) noexcept;

	/// <summary>
	/// アフィン変換行列を作成
	/// </summary>
	/// <param name="scale">拡縮</param>
	/// <param name="sinTheta">回転</param>
	/// <param name="cosTheta">回転</param>
	/// <param name="translate">移動</param>
	/// <returns></returns>
	static constexpr Matrix3x3 MakeAffineMatrix(const Vector2& scale, const float sinTheta, const float cosTheta, const Vector2& translate) noexcept;

	/// <summary>
	/// 2次元ベクトルの同次座標系乗算
	/// </summary>
	/// <param name="vector">変換する2次元ベクトル</param>
	/// <param name="matrix">変換行列</param>
	/// <returns></returns>
	static constexpr Vector2 Homogeneous(const Vector2& vector, const Matrix3x3& matrix);

	/// <summary>
	/// 2次元ベクトルの同次座標系乗算(Translateなし)
	/// </summary>
	/// <param name="vector">変換する2次元ベクトル</param>
	/// <param name="matrix">変換行列</param>
	/// <returns></returns>
	static constexpr Vector2 HomogeneousVector(const Vector2& vector, const Matrix3x3& matrix);
};

constexpr Matrix3x3 Transform2D::MakeIdentityMatrix() noexcept {
	return {
		{{ 1, 0, 0 },
		{ 0, 1, 0 },
		{ 0, 0, 1 } }
	};
}

constexpr Matrix3x3 Transform2D::MakeRotateMatrix(const float sine, const float cosine) noexcept {
	return {
		{{ cosine, sine, 0 },
		{ -sine, cosine, 0 },
		{ 0, 0, 1 } }
	};
}

constexpr Matrix3x3 Transform2D::MakeScaleMatrix(const float x, const float y) noexcept {
	return {
		{{ x, 0, 0 },
		{ 0, y, 0 },
		{ 0, 0, 1 }}
	};
}

constexpr Matrix3x3 Transform2D::MakeScaleMatrix(const Vector2& scale) noexcept {
	return MakeScaleMatrix(scale.x, scale.y);
}

constexpr Matrix3x3 Transform2D::MakeTranslateMatrix(const float x, const float y) noexcept {
	return{ {
		{ 1, 0, 0 },
		{ 0, 1, 0 },
		{ x, y, 1 }
		} };
}

constexpr Matrix3x3 Transform2D::MakeTranslateMatrix(const Vector2& translate) noexcept {
	return MakeTranslateMatrix(translate.x, translate.y);
}

constexpr Vector2 Transform2D::Homogeneous(const Vector2& vector, const Matrix3x3& matrix) {
	float w = vector.x * matrix[0][2] + vector.y * matrix[1][2] + 1.0f * matrix[2][2];
	assert(w != 0);
	return { (vector.x * matrix[0][0] + vector.y * matrix[1][0] + 1.0f * matrix[2][0]) / w, (vector.x * matrix[0][1] + vector.y * matrix[1][1] + 1.0f * matrix[2][1]) / w };;
}

constexpr Vector2 Transform2D::HomogeneousVector(const Vector2& vector, const Matrix3x3& matrix) {
	float w = vector.x * matrix[0][2] + vector.y * matrix[1][2] + 1.0f * matrix[2][2];
	assert(w != 0);
	return { (vector.x * matrix[0][0] + vector.y * matrix[1][0] + 1.0f * 0) / w, (vector.x * matrix[0][1] + vector.y * matrix[1][1] + 1.0f * 0) / w };
}