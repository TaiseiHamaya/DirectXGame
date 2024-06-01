#pragma once

#include "Engine/Math/Matrix.h"
#include "Engine/Math/Vector2.h"

class Transform2D final {
public:
	Transform2D() noexcept;
	Transform2D(const Vector2& scale_, float rotate_, const Vector2& translate_) noexcept;
	Transform2D(Vector2&& scale_, float rotate_, Vector2&& translate_) noexcept;
	~Transform2D() = default;

public:
	inline Transform2D& operator=(const Transform2D& opr) noexcept {
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

	inline Transform2D& operator=(Transform2D&& opr) noexcept {
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

public:
	void set_scale(const Vector2& scale_);
	void set_rotate(float rotate_);
	void set_rotate(float sintheta_, float costheta_);
	void set_translate(const Vector2& translate_);
	void set_translate_x(float x);
	void set_translate_y(float y);
	void begin();
	Matrix3x3 get_matrix() const;
	Matrix4x4 get_matrix4x4_transform() const;
	Matrix4x4 get_matrix4x4_padding() const;
	const Vector2& get_translate() const;
	void plus_translate(const Vector2& plus);

	bool need_update_matrix() const;

#ifdef _DEBUG
public:
	void debug_gui(float translateMove = 0.01f);
#endif // _DEBUG

private:
	Vector2 scale;
	float rotate;
	Vector2 translate;
	float sinTheta;
	float cosTheta;

	bool isNeedUpdate;

public:// static関数
	/// <summary>
	/// 単位行列を作成
	/// </summary>
	/// <returns></returns>
	static Matrix3x3 MakeIdentityMatrix();

	/// <summary>
	/// 回転行列を作成
	/// </summary>
	/// <param name="theta">回転</param>
	/// <returns></returns>
	static Matrix3x3 MakeRotateMatrix(const float theta);

	/// <summary>
	/// 回転行列を作成
	/// </summary>
	/// <param name="sinTheta">回転</param>
	/// <param name="cosTheta">回転</param>
	/// <returns></returns>
	static Matrix3x3 MakeRotateMatrix(const float sine, const float cosine);

	/// <summary>
	/// 拡縮行列を作成
	/// </summary>
	/// <param name="x">拡縮X</param>
	/// <param name="y">拡縮Y</param>
	/// <returns></returns>
	static Matrix3x3 MakeScaleMatrix(const float x, const float y);

	/// <summary>
	/// 拡縮行列を作成
	/// </summary>
	/// <param name="scale">拡縮</param>
	/// <returns></returns>
	static Matrix3x3 MakeScaleMatrix(const Vector2& scale);

	/// <summary>
	/// 平行移動行列を作成
	/// </summary>
	/// <param name="x">移動X</param>
	/// <param name="y">移動Y</param>
	/// <returns></returns>
	static Matrix3x3 MakeTranslateMatrix(const float x, const float y);

	/// <summary>
	/// 平行移動行列を作成
	/// </summary>
	/// <param name="translate">移動</param>
	/// <returns></returns>
	static Matrix3x3 MakeTranslateMatrix(const Vector2& translate);

	/// <summary>
	/// アフィン変換行列を作成
	/// </summary>
	/// <param name="scale">拡縮</param>
	/// <param name="theta">回転</param>
	/// <param name="translate">移動</param>
	/// <returns></returns>
	static Matrix3x3 MakeAffineMatrix(const Vector2& scale, const float theta, const Vector2& translate);

	/// <summary>
	/// アフィン変換行列を作成
	/// </summary>
	/// <param name="scale">拡縮</param>
	/// <param name="sinTheta">回転</param>
	/// <param name="cosTheta">回転</param>
	/// <param name="translate">移動</param>
	/// <returns></returns>
	static Matrix3x3 MakeAffineMatrix(const Vector2& scale, const float sinTheta, const float cosTheta, const Vector2& translate);

	/// <summary>
	/// 2次元ベクトルの同次座標系乗算
	/// </summary>
	/// <param name="vector">変換する2次元ベクトル</param>
	/// <param name="matrix">変換行列</param>
	/// <returns></returns>
	static Vector2 Homogeneous(const Vector2& vector, const Matrix3x3& matrix);

	/// <summary>
	/// 2次元ベクトルの同次座標系乗算(Translateなし)
	/// </summary>
	/// <param name="vector">変換する2次元ベクトル</param>
	/// <param name="matrix">変換行列</param>
	/// <returns></returns>
	static Vector2 HomogeneousVector(const Vector2& vector, const Matrix3x3& matrix);
};