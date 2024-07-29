#pragma once

#include "Vector3.h"
#include "Matrix4x4.h"

class Quaternion final {
public:
	Quaternion() noexcept;
	~Quaternion() noexcept = default;

	/// <summary>
	/// クォータニオンのパラメータを直接構築して作成
	/// </summary>
	/// <param name="xyz_">虚部</param>
	/// <param name="w_">実部</param>
	Quaternion(const Vector3& xyz_, float w_) noexcept;

	/// <summary>
	/// クォータニオンのパラメータを直接構築して作成
	/// </summary>
	/// <param name="x">i成分</param>
	/// <param name="y">j成分</param>
	/// <param name="z">k成分</param>
	/// <param name="w">実部</param>
	Quaternion(float x, float y, float z, float w) noexcept;

	Quaternion(const Quaternion& rhs) noexcept = default;
	Quaternion(Quaternion&& rhs) noexcept = default;
	Quaternion& operator=(const Quaternion& rhs) noexcept = default;
	Quaternion& operator=(Quaternion&& rhs) noexcept = default;

private:
	Vector3 xyz; // 虚部ベクトル
	float w; // 実部

public:
	/// <summary>
	/// 回転軸と回転量を指定してQuaternionを作成
	/// </summary>
	/// <param name="axis">回転軸(Vector3)</param>
	/// <param name="angleAxis">回転量(Radian)</param>
	static const Quaternion AngleAxis(const Vector3& axis, float angleAxis);

	/// <summary>
	/// オイラー角からクォータニオンを生成(ラジアン)
	/// </summary>
	/// <param name="pitch">X軸回転</param>
	/// <param name="yaw">Y軸回転</param>
	/// <param name="roll">Z軸回転</param>
	static const Quaternion EulerRadian(float pitch, float yaw, float roll) noexcept;

	/// <summary>
	/// オイラー角からクォータニオンを生成(ラジアン)
	/// </summary>
	/// <param name="rotate">XYZ軸回転</param>
	static const Quaternion EulerRadian(const Vector3& rotate) noexcept;

	/// <summary>
	/// オイラー角からクォータニオンを生成(度数法)
	/// </summary>
	/// <param name="rotate">XYZ軸回転</param>
	/// <returns></returns>
	static const Quaternion EulerDegree(const Vector3& rotate) noexcept;

	/// <summary>
	/// オイラー角からクォータニオンを生成(度数法)
	/// </summary>
	/// <param name="pitch">X軸回転</param>
	/// <param name="yaw">Y軸回転</param>
	/// <param name="roll">Z軸回転</param>
	/// <returns></returns>
	static const Quaternion EulerDegree(float pitch, float yaw, float roll) noexcept;


public:
	bool operator==(const Quaternion& rhs) const noexcept;
	bool operator!=(const Quaternion& rhs) const noexcept;
	Quaternion operator*(const Quaternion& rhs) const noexcept;
	Quaternion& operator*=(const Quaternion& rhs) noexcept;
	Quaternion operator*(float times) const noexcept;
	Quaternion& operator*=(float times) noexcept;

public: // メンバ関数
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
	/// クォータニオンの正規化
	/// </summary>
	/// <returns></returns>
	const Quaternion normalize() const noexcept;

	/// <summary>
	/// ベクトル部の取得
	/// </summary>
	/// <returns></returns>
	const Vector3& vector() const noexcept;

public: // グローバルメンバ関数
	static const Quaternion FromToRotation(const Vector3& from, const Vector3& to);

	static const Quaternion LookForward(const Vector3& forward, const Vector3& upwards = CVector3::BASIS_Y);

	/// <summary>
	/// 球面線形補間
	/// </summary>
	/// <param name="internal">開始Quaternion</param>
	/// <param name="terminal">終了Quaternion</param>
	/// <param name="t">媒介変数</param>
	/// <returns>変換後Quaternion</returns>
	static const Quaternion Slerp(const Quaternion& internal, const Quaternion& terminal, float t) noexcept;
};

namespace CQuaternion {
	static const Quaternion IDENTITY{ 0,0,0,1 };
};

const Vector3 operator*(const Vector3& vector, const Quaternion& quaternion);
