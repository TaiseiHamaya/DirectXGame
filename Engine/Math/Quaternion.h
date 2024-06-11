#pragma once

#include "Vector3.h"
#include "Matrix4x4.h"

class Quaternion final {
public:
	Quaternion() noexcept;
	~Quaternion() noexcept = default;

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
	Quaternion(float x, float y, float z, float w) noexcept;

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

	Quaternion(const Quaternion& rhs) noexcept;
	Quaternion& operator=(const Quaternion& rhs) noexcept;

	Quaternion(Quaternion&& rhs) noexcept;
	Quaternion& operator=(Quaternion&& rhs) noexcept;

public:
	bool operator==(const Quaternion& rhs) const noexcept;
	bool operator!=(const Quaternion& rhs) const noexcept;
	Quaternion operator*(const Quaternion& rhs) const noexcept;
	Quaternion& operator*=(const Quaternion& rhs) noexcept;
	Quaternion operator*(float times) const noexcept;
	Quaternion& operator*=(float times) noexcept;

public:
	/// <summary>
	/// 回転行列に変換
	/// </summary>
	/// <returns>回転行列Matrix4x4</returns>
	const Matrix4x4 to_matrix() const noexcept;

	/// <summary>
	/// Quaternionベクトルの長さ[1]
	/// </summary>
	/// <returns>基本は1</returns>
	const float length() const noexcept;

	/// <summary>
	/// 逆クォータニオンの取得
	/// </summary>
	/// <returns>逆Quaternion</returns>
	const Quaternion inverse() const noexcept;

	/// <summary>
	/// 球面線形補間
	/// </summary>
	/// <param name="internal">開始Quaternion</param>
	/// <param name="terminal">終了Quaternion</param>
	/// <param name="t">媒介変数</param>
	/// <returns>変換後Quaternion</returns>
	static const Quaternion Slerp(const Quaternion& internal, const Quaternion& terminal, float t) noexcept;

	/// <summary>
	/// [0,0,0,1]のQuaternion
	/// </summary>
	/// <returns>Quaternion</returns>
	static const Quaternion& Identity() noexcept;

private:
	Vector3 xyz; // 虚部ベクトル
	float w; // 実部
};
