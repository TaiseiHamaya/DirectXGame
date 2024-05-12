#pragma once

#include "Vector3.h"
#include "Matrix.h"

class Quaternion final {
public:
	Quaternion() noexcept;
	~Quaternion() = default;

	Quaternion(const Quaternion& rhs) noexcept;
	Quaternion(Quaternion&& rhs) noexcept;

	/// <summary>
	/// 回転軸と回転量を指定してQuaternionを作成
	/// </summary>
	/// <param name="axis">回転軸(Vector3)</param>
	/// <param name="angleAxis">回転量(Radian)</param>
	Quaternion(const Vector3& axis, float angleAxis);

	/// <summary>
	/// クォータニオンのパラメータを直接構築して作成
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	/// <param name="w"></param>
	Quaternion(float x, float y, float z, float w);

	/// <summary>
	/// オイラー角からクォータニオンを生成(ラジアン)
	/// </summary>
	/// <param name="pitch">X軸回転</param>
	/// <param name="yaw">Y軸回転</param>
	/// <param name="roll">Z軸回転</param>
	Quaternion(float pitch, float yaw, float roll) noexcept;

	/// <summary>
	/// オイラー角からクォータニオンを生成(ラジアン)
	/// </summary>
	/// <param name="rotate">XYZ軸回転</param>
	Quaternion(const Vector3& rotate) noexcept;

public:
	Quaternion& operator=(const Quaternion& rhs) noexcept;
	Quaternion& operator=(Quaternion&& rhs) noexcept;
	bool operator==(const Quaternion& rhs) const noexcept;
	bool operator!=(const Quaternion& rhs) const noexcept;
	Quaternion operator*(const Quaternion& rhs) const;
	Quaternion& operator*=(const Quaternion& rhs);
	Quaternion operator*(float times) const;
	Quaternion& operator*=(float times);

public:
	/// <summary>
	/// 回転行列に変換
	/// </summary>
	/// <returns></returns>
	const Matrix4x4 to_matrix() const;

	const float length() const;

	const Quaternion inverse() const;

	static const Quaternion Slerp(const Quaternion& internal, const Quaternion& terminal, float t);

	static const Quaternion& Identity();

private:
	Vector3 xyz;
	float w;
};
