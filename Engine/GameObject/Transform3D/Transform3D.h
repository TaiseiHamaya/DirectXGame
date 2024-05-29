#pragma once

#include "Engine/Math/Matrix.h"
#include "Engine/Math/Quaternion.h"
#include "Engine/Math/Vector3.h"

class Transform3D final {
public:
	Transform3D() noexcept;
	Transform3D(const Vector3& scale_, const Quaternion& quaternion, const Vector3& translate_) noexcept;
	Transform3D(Vector3&& scale_, Quaternion&& quaternion, Vector3&& translate_) noexcept;
	~Transform3D() = default;

public:
	Transform3D(const Transform3D&) = delete;
	Transform3D& operator=(const Transform3D&) = delete;

public:
	void set_scale(const Vector3& scale_);
	void set_rotate(const Quaternion& rotate_);
	void set_translate(const Vector3& translate_);
	void set_translate_x(float x);
	void set_translate_y(float y);
	void set_translate_z(float z);
	void begin();

	Matrix4x4 get_matrix() const;
	const Vector3& get_scale() const;
	const Vector3& get_translate() const;
	const Quaternion& get_quaternion() const;
	void plus_translate(const Vector3& plus);

	bool need_update_matrix() const;

	void debug_gui();
	//void debug_axis(const Matrix4x4& debug_matrix) const;

private:
	//friend const Vector3& Quaternion::get_axis();

private:
	Vector3 scale;
	Quaternion rotate;
	Vector3 translate;

	bool isNeedUpdate;

public:// static関数
	/// <summary>
	/// 単位行列を作成
	/// </summary>
	/// <returns></returns>
	static Matrix4x4 MakeIdentityMatrix();

	/// <summary>
	/// X軸方向の回転行列を作成
	/// </summary>
	/// <param name="theta">X軸回転</param>
	/// <returns></returns>
	static Matrix4x4 MakeRotateXMatrix(const float theta);

	/// <summary>
	/// Y軸方向の回転行列を作成
	/// </summary>
	/// <param name="theta">Y軸回転</param>
	/// <returns></returns>
	static Matrix4x4 MakeRotateYMatrix(const float theta);

	/// <summary>
	/// Y軸方向の回転行列を作成
	/// </summary>
	/// <param name="theta">Y軸回転</param>
	/// <returns></returns>
	static Matrix4x4 MakeRotateZMatrix(const float theta);

	/// <summary>
	/// 回転行列を作成
	/// </summary>
	/// <param name="x">X軸回転量</param>
	/// <param name="y">Y軸回転量</param>
	/// <param name="z">Z軸回転量</param>
	/// <returns></returns>
	static Matrix4x4 MakeRotateMatrix(const float x, const float y, const float z);

	/// <summary>
	/// 拡縮行列を作成
	/// </summary>
	/// <param name="x">拡縮X</param>
	/// <param name="y">拡縮Y</param>
	/// <param name="z">拡縮Z</param>
	/// <returns></returns>
	static Matrix4x4 MakeScaleMatrix(const float x, const float y, const float z);

	/// <summary>
	/// 拡縮行列を作成
	/// </summary>
	/// <param name="scale">拡縮</param>
	/// <returns></returns>
	static Matrix4x4 MakeScaleMatrix(const Vector3& scale);

	/// <summary>
	/// 平行移動行列を作成
	/// </summary>
	/// <param name="x">移動X</param>
	/// <param name="y">移動Y</param>
	/// <param name="z">移動Z</param>
	/// <returns></returns>
	static Matrix4x4 MakeTranslateMatrix(const float x, const float y, const float z);

	/// <summary>
	/// 平行移動行列を作成
	/// </summary>
	/// <param name="translate">移動</param>
	/// <returns></returns>
	static Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

	/// <summary>
	/// アフィン変換行列を作成
	/// </summary>
	/// <param name="scale">拡縮</param>
	/// <param name="theta">回転</param>
	/// <param name="translate">移動</param>
	/// <returns></returns>
	static Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Quaternion& rotate, const Vector3& translate);

	/// <summary>
	/// アフィン変換行列を作成
	/// </summary>
	/// <param name="scale">拡縮</param>
	/// <param name="sinTheta">回転</param>
	/// <param name="cosTheta">回転</param>
	/// <param name="translate">移動</param>
	/// <returns></returns>
	static Matrix4x4 MakeAffineMatrix(const Vector3& scale, const float sinTheta, const float cosTheta, const Vector3& translate);

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
};