#pragma once

#include "Engine/Math/Matrix4x4.h"
#include "Engine/Math/Quaternion.h"
#include "Engine/Math/Vector3.h"

class Transform3D final {
public:
	Transform3D() noexcept;
	Transform3D(const Vector3& scale_, const Quaternion& quaternion, const Vector3& translate_) noexcept;
	Transform3D(Vector3&& scale_, Quaternion&& quaternion, Vector3&& translate_) noexcept;
	~Transform3D() noexcept = default;

public:
	Transform3D(const Transform3D&) = delete;
	Transform3D& operator=(const Transform3D&) = delete;
	Transform3D(Transform3D&&) noexcept = default;
	Transform3D& operator=(Transform3D&&) noexcept = default;

public:
	void set_scale(const Vector3& scale_) noexcept;
	void set_rotate(const Quaternion& rotate_) noexcept;
	void set_translate(const Vector3& translate_) noexcept;
	void set_translate_x(float x) noexcept;
	void set_translate_y(float y) noexcept;
	void set_translate_z(float z) noexcept;

	Matrix4x4 get_matrix() const noexcept;
	const Vector3& get_scale() const noexcept;
	const Vector3& get_translate() const noexcept;
	const Quaternion& get_quaternion() const noexcept;
	void plus_translate(const Vector3& plus) noexcept;

	void copy(const Transform3D& copy) noexcept;
	void look_at(const Transform3D& rhs, const Vector3& upwards = CVector3::BASIS_Y) noexcept;

#ifdef _DEBUG
	void debug_gui();
	//void debug_axis(const Matrix4x4& debug_matrix) const;
#endif // _DEBUG

private:
	Vector3 scale;
	Quaternion rotate;
	Vector3 translate;

public:// static関数
	/// <summary>
	/// 単位行列を作成
	/// </summary>
	/// <returns></returns>
	static constexpr Matrix4x4 MakeIdentityMatrix() noexcept;

	/// <summary>
	/// X軸方向の回転行列を作成
	/// </summary>
	/// <param name="theta">X軸回転</param>
	/// <returns></returns>
	static Matrix4x4 MakeRotateXMatrix(const float theta) noexcept;

	/// <summary>
	/// Y軸方向の回転行列を作成
	/// </summary>
	/// <param name="theta">Y軸回転</param>
	/// <returns></returns>
	static Matrix4x4 MakeRotateYMatrix(const float theta) noexcept;

	/// <summary>
	/// Y軸方向の回転行列を作成
	/// </summary>
	/// <param name="theta">Y軸回転</param>
	/// <returns></returns>
	static Matrix4x4 MakeRotateZMatrix(const float theta) noexcept;

	/// <summary>
	/// 回転行列を作成
	/// </summary>
	/// <param name="x">X軸回転量</param>
	/// <param name="y">Y軸回転量</param>
	/// <param name="z">Z軸回転量</param>
	/// <returns></returns>
	static Matrix4x4 MakeRotateMatrix(const float x, const float y, const float z) noexcept;
	
	static Matrix4x4 MakeRotateMatrix(const Vector3& rotate) noexcept;

	/// <summary>
	/// 拡縮行列を作成
	/// </summary>
	/// <param name="x">拡縮X</param>
	/// <param name="y">拡縮Y</param>
	/// <param name="z">拡縮Z</param>
	/// <returns></returns>
	static constexpr Matrix4x4 MakeScaleMatrix(const float x, const float y, const float z) noexcept;

	/// <summary>
	/// 拡縮行列を作成
	/// </summary>
	/// <param name="scale">拡縮</param>
	/// <returns></returns>
	static constexpr Matrix4x4 MakeScaleMatrix(const Vector3& scale) noexcept;

	/// <summary>
	/// 平行移動行列を作成
	/// </summary>
	/// <param name="x">移動X</param>
	/// <param name="y">移動Y</param>
	/// <param name="z">移動Z</param>
	/// <returns></returns>
	static constexpr Matrix4x4 MakeTranslateMatrix(const float x, const float y, const float z) noexcept;

	/// <summary>
	/// 平行移動行列を作成
	/// </summary>
	/// <param name="translate">移動</param>
	/// <returns></returns>
	static constexpr Matrix4x4 MakeTranslateMatrix(const Vector3& translate) noexcept;

	/// <summary>
	/// アフィン変換行列を作成
	/// </summary>
	/// <param name="scale">拡縮</param>
	/// <param name="theta">回転</param>
	/// <param name="translate">移動</param>
	/// <returns></returns>
	static Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Quaternion& rotate, const Vector3& translate) noexcept;
	
	static Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) noexcept;

	/// <summary>
	/// 2次元ベクトルの同次座標系乗算
	/// </summary>
	/// <param name="vector">変換する3次元ベクトル</param>
	/// <param name="matrix">変換行列</param>
	/// <returns></returns>
	static Vector3 Homogeneous(const Vector3& vector, const Matrix4x4& matrix);

	/// <summary>
	/// 2次元ベクトルの同次座標系乗算(Translateなし)
	/// </summary>
	/// <param name="vector">変換する3次元ベクトル</param>
	/// <param name="matrix">変換行列</param>
	/// <returns></returns>
	static Vector3 HomogeneousVector(const Vector3& vector, const Matrix4x4& matrix);

	static Vector3 ExtractPosition(const Matrix4x4& matrix);
};

constexpr Matrix4x4 Transform3D::MakeScaleMatrix(const float x, const float y, const float z) noexcept {
	return { {
		{ x, 0, 0, 0 },
		{ 0, y, 0, 0 },
		{ 0, 0, z, 0 },
		{ 0, 0, 0, 1 }
		} };
}

constexpr Matrix4x4 Transform3D::MakeScaleMatrix(const Vector3& scale) noexcept {
	return MakeScaleMatrix(scale.x, scale.y, scale.z);
}

constexpr Matrix4x4 Transform3D::MakeTranslateMatrix(const float x, const float y, const float z) noexcept {
	return { {
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ x, y, z, 1 }
		} };
}

constexpr Matrix4x4 Transform3D::MakeTranslateMatrix(const Vector3& translate) noexcept {
	return MakeTranslateMatrix(translate.x, translate.y, translate.z);
}