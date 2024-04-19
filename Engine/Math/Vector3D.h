#pragma once

#include <utility>

/// <summary>
/// 3次元ベクトルクラス
/// <param name="x">x方向のベクトル</param>
/// <param name="y">y方向のベクトル</param>
/// <param name="z">z方向のベクトル</param>
/// </summary>
class Vector3 final { // 3次元ベクトル
public:
	constexpr Vector3() noexcept;
	constexpr Vector3(float x, float y, float z) noexcept;
	constexpr Vector3(const Vector3& vec) noexcept;
	constexpr Vector3(Vector3&& vec) noexcept;

	~Vector3() = default;

public:
	// ------------------基礎構造------------------
	float x; // X座標
	float y; // Y座標
	float z; // Z座標

public:
	// ------------------operator------------------
	inline constexpr Vector3& operator=(const Vector3& vec) noexcept;
	inline constexpr Vector3& operator=(Vector3&& vec) noexcept;
	inline constexpr Vector3 operator+(void) const noexcept;
	inline constexpr Vector3 operator-(void) const noexcept;
	inline constexpr Vector3 operator+(const Vector3& opr) const noexcept;
	inline constexpr Vector3 operator-(const Vector3& opr) const noexcept;
	inline constexpr Vector3 operator*(float times) const noexcept;
	inline constexpr Vector3 operator/(float times) const;
	inline Vector3& operator+=(const Vector3& opr) noexcept;
	inline Vector3& operator-=(const Vector3& opr) noexcept;
	inline Vector3& operator*=(float times) noexcept;
	inline Vector3& operator/=(float times);
	inline constexpr bool operator==(const Vector3& opr) const noexcept;
	inline constexpr bool operator!=(const Vector3& opr) const noexcept;


public:
	// ------------------便利関数------------------

	/// <summary>
	/// ベクトルの長さを出す
	/// </summary>
	/// <returns></returns>
	float length(void) const;

	/// <summary>
	/// ベクトルの正規化
	/// </summary>
	/// <returns></returns>
	Vector3 normalize(void) const;

public:
	// ------------------static関数------------------

	/// <summary>
	/// 2つのベクトルの内積を計算
	/// <param name="vector1">3次元ベクトル1</param>
	/// <param name="vector2">3次元ベクトル2</param>
	/// </summary>
	static float DotProduct(const Vector3& vector1, const Vector3& vector2);

	/// <summary>
	/// 2つのベクトルの外積を計算
	/// <param name="vector1">3次元ベクトル1</param>
	/// <param name="vector2">3次元ベクトル2</param>
	/// </summary>
	static Vector3 CrossProduct(const Vector3& vector1, const Vector3& vector2);

	/// <summary>
	/// ベクトルの大きさを計算
	/// <param name="vector">終点の座標</param>
	/// </summary>
	static float Length(const Vector3& vector);

	/// <summary>
	/// ベクトルの大きさを計算
	/// <param name="vector1">終点の座標</param>
	/// <param name="vector2">始点の座標</param>
	/// </summary>
	static float Length(const Vector3& vector1, const Vector3& vector2);

	/// <summary>
	/// ベクトルの正規化
	/// <param name="vector1">終点の座標</param>
	/// </summary>
	static Vector3 Normalize(const Vector3& vector);

	/// <summary>
	/// ベクトルの正規化
	/// <param name="vector1">始点の座標</param>
	/// <param name="vector2">終点の座標</param>
	/// </summary>
	static Vector3 Normalize(const Vector3& vectorFrom, const Vector3& vectorTo);

	/// <summary>
	/// 2つのベクトルを加算
	/// <param name="vector1">足されるベクトル</param>
	/// <param name="vector2">足すベクトル</param>
	/// </summary>
	static Vector3 Add(const Vector3& vector1, const Vector3& vector2);

	/// <summary>
	/// 2つのベクトルを減算
	/// <param name="vector1">引かれるベクトル</param>
	/// <param name="vector2">引くベクトル</param>
	/// </summary>
	static Vector3 Subtract(const Vector3& vector1, const Vector3& vector2);

	/// <summary>
	/// ベクトルを定数倍
	/// <param name="vector">乗算されるベクトル</param>
	/// <param name="times">係数</param>
	/// </summary>
	static Vector3 Multiply(const Vector3& vector, const float& times);

	/// <summary>
	/// ベクトルを定数倍
	/// <param name="vector1">乗算されるベクトル</param>
	/// <param name="times">係数</param>
	/// </summary>
	static Vector3 Multiply(const Vector3& vector, const Vector3& times);

	/// <summary>
	/// 2つの位置ベクトルの間の座標
	/// <param name="from">始点の位置ベクトル</param>
	/// <param name="to">終点の位置ベクトル</param>
	/// <param name="t">割合T</param>
	/// </summary>
	static Vector3 Lerp(const Vector3& from, const Vector3& to, const float& t);

	/// <summary>
	/// <param name="vector">変換するベクトル</param>
	/// <param name="radian">回転量(ラジアン)</param>
	/// </summary>
	//static Vector3 Rotate(const Vector3& vector, const float& radian);

	/// <summary>
	/// <param name="vector">変換するベクトル</param>
	/// <param name="radian">回転量(ラジアン)</param>
	/// </summary>
	//static Vector3 Rotate(const Vector3& vector, const float sintheta, const float costheta);

	/// <summary>
	/// 2次ベジエ曲線
	/// </summary>
	/// <param name="initial">始点</param>
	/// <param name="control">制御点</param>
	/// <param name="terminal">終点</param>
	/// <param name="t">媒介変数</param>
	/// <returns></returns>
	static Vector3 Bezier(const Vector3& initial, const Vector3& control, const Vector3& terminal, const float t);

	/// <summary>
	/// ベクトルの各要素の絶対値化
	/// </summary>
	/// <param name="vector">変化させるベクトル</param>
	/// <returns></returns>
	static Vector3 Abs(const Vector3& vector);
};

// ------------------インライン関数定義------------------
inline constexpr Vector3::Vector3() noexcept :
	x(0.0f),
	y(0.0f),
	z(0.0f) {
}

inline constexpr Vector3::Vector3(float x, float y, float z) noexcept :
	x(x),
	y(y),
	z(z) {
}

inline constexpr Vector3::Vector3(const Vector3& vec) noexcept :
	x(vec.x),
	y(vec.y),
	z(vec.z) {
}

inline constexpr Vector3::Vector3(Vector3&& vec) noexcept :
	x(std::move(vec.x)),
	y(std::move(vec.y)),
	z(std::move(vec.z)) {
}

inline constexpr Vector3& Vector3::operator=(const Vector3& vec) noexcept {
	x = vec.x;
	y = vec.y;
	z = vec.z;
	return *this;
}

inline constexpr Vector3& Vector3::operator=(Vector3&& vec) noexcept {
	x = std::move(vec.x);
	y = std::move(vec.y);
	z = std::move(vec.z);
	return *this;
}

inline constexpr Vector3 Vector3::operator+() const noexcept {
	return *this;
}

inline constexpr Vector3 Vector3::operator-() const noexcept {
	return { -x, -y, -z };
}

inline constexpr Vector3 Vector3::operator+(const Vector3& opr) const noexcept {
	return { x + opr.x, y + opr.y, z + opr.z };
}

inline constexpr Vector3 Vector3::operator-(const Vector3& opr) const noexcept {
	return { x - opr.x, y - opr.y, z - opr.z };
}

inline constexpr Vector3 Vector3::operator*(float times) const noexcept {
	return { x * times, y * times, z * times };
}

inline constexpr Vector3 Vector3::operator/(float times) const {
	return { x / times, y / times, z / times };
}

inline Vector3& Vector3::operator+=(const Vector3& opr) noexcept {
	x += opr.x;
	y += opr.y;
	z += opr.z;
	return *this;
}

inline Vector3& Vector3::operator-=(const Vector3& opr) noexcept {
	x -= opr.x;
	y -= opr.y;
	z -= opr.z;
	return *this;
}

inline Vector3& Vector3::operator*=(float times) noexcept {
	x *= times;
	y *= times;
	z *= times;
	return *this;
}

inline Vector3& Vector3::operator/=(float times) {
	x /= times;
	y /= times;
	z /= times;
	return *this;
}

inline constexpr bool Vector3::operator==(const Vector3& opr) const noexcept {
	return x == opr.x && y == opr.y && z == opr.z;
}

inline constexpr bool Vector3::operator!=(const Vector3& opr) const noexcept {
	return !(*this == opr);
}

/// <summary>
/// Vector3定数
/// </summary>
namespace Vec3 {
	constexpr Vector3 kBasisX = Vector3{ 1.0f, 0.0f, 0.0f }; // x(1.0f), y(0.0f), z(0.0f)
	constexpr Vector3 kBasisY = Vector3{ 0.0f, 1.0f, 0.0f }; // x(0.0f), y(1.0f), z(0.0f)
	constexpr Vector3 kBasisZ = Vector3{ 0.0f, 0.0f, 1.0f }; // x(0.0f), y(0.0f), z(1.0f)
	constexpr Vector3 kZero = Vector3{ 0.0f, 0.0f, 0.0f }; // x(0.0f), y(0.0f), z(0.0f)
	constexpr Vector3 kBasis = Vector3{ 1.0f, 1.0f, 1.0f }; // x(1.0f), y(1.0f), z(1.0f)
}