#pragma once

#include "Vector2D.h"
#include "Matrix.h"
#include "Definition.h"

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
			sintheta != opr.sintheta ||
			costheta != opr.costheta
			) {
			isNeedUpdate = true;
		}
		else {
			isNeedUpdate = false;
		}
		scale = opr.scale;
		translate = opr.translate;
		sintheta = opr.sintheta;
		costheta = opr.costheta;
		matrix = opr.matrix;
		return *this;
	}

	inline Transform2D& operator=(Transform2D&& opr) noexcept {
		if (scale != opr.scale ||
			translate != opr.translate ||
			sintheta != opr.sintheta ||
			costheta != opr.costheta
			) {
			isNeedUpdate = true;
		}
		else {
			isNeedUpdate = false;
		}
		scale = std::move(opr.scale);
		translate = std::move(opr.translate);
		sintheta = std::move(opr.sintheta);
		costheta = std::move(opr.costheta);
		matrix = std::move(opr.matrix);
		return *this;
	}

public:
	void set_scale(const Vector2& scale_);
	void set_rotate(float rotate_);
	void set_rotate(float sintheta_, float costheta_);
	void set_translate(const Vector2& translate_);
	void set_translatex(float x);
	void set_translatey(float y);
	void begin();
	void update();
	const Matrix3x3& get_matrix() const;
	const Matrix4x4 get_matrix4x4() const;
	const Vector2& get_translate() const;
	void plus_translate(const Vector2& plus);

	bool need_update_matrix() const;

#ifdef _DEBUG
public:
	void debug_gui();
#endif // _DEBUG

private:
	Vector2 scale;
	float rotate;
	Vector2 translate;
	float sintheta;
	float costheta;
	//union Rotate {
	//	struct Radian {
	//		float sintheta;
	//		float costheta;
	//	};
	//	Vector2 direction;
	//	Radian radian;
	//};
	//Rotate rotate1;
	Matrix3x3 matrix;

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
	/// <param name="sintheta">回転</param>
	/// <param name="costheta">回転</param>
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
	/// <param name="sintheta">回転</param>
	/// <param name="costheta">回転</param>
	/// <param name="translate">移動</param>
	/// <returns></returns>
	static Matrix3x3 MakeAffineMatrix(const Vector2& scale, const float sintheta, const float costheta, const Vector2& translate);

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

	/// <summary>
	/// 長方形構造体を同次座標系乗算
	/// </summary>
	/// <param name="rect">変換する長方形</param>
	/// <param name="matrix">変換行列</param>
	/// <returns></returns>
	static Rect Homogeneous(const Rect& rect, const Matrix3x3& matrix);

	/// <summary>
	/// 長方形構造体を同次座標系乗算(Translateなし)
	/// </summary>
	/// <param name="rect">変換する長方形</param>
	/// <param name="matrix">変換行列</param>
	/// <returns></returns>
	static Rect HomogeneousVector(const Rect& rect, const Matrix3x3& matrix);
};