#pragma once

#include <limits>

#include "Vector3.h"

/// <summary>
/// 2次元ベクトル構造体
/// <param name="x">x方向のベクトル</param>
/// <param name="y">y方向のベクトル</param>
/// </summary>
class Vector2 final { // 2次元ベクトル
	/// <summary>
	/// コンストラクタ
	/// </summary>
public:
	inline constexpr Vector2() noexcept;
	inline constexpr Vector2(float x, float y) noexcept;
	inline constexpr Vector2(const Vector2& vec) noexcept = default;
	inline constexpr Vector2(Vector2&& vec) noexcept = default;

	~Vector2() noexcept = default;

	/// <summary>
	/// 基礎構造
	/// </summary>
public:
	float x; // X座標
	float y; // Y座標

	/// <summary>
	/// operator
	/// </summary>
public:
	inline constexpr Vector2& operator=(const Vector2& vec) noexcept = default;
	inline constexpr Vector2& operator=(Vector2&& vec) noexcept = default;
	inline constexpr const Vector2 operator+(void) const noexcept;
	inline constexpr const Vector2 operator-(void) const noexcept;
	inline constexpr const Vector2 operator+(const Vector2& opr) const noexcept;
	inline constexpr const Vector2 operator-(const Vector2& opr) const noexcept;
	inline constexpr const Vector2 operator*(float times) const noexcept;
	inline constexpr const Vector2 operator/(float times) const noexcept;
	inline constexpr Vector2& operator+=(const Vector2& opr) noexcept;
	inline constexpr Vector2& operator-=(const Vector2& opr) noexcept;
	inline constexpr Vector2& operator*=(float times) noexcept;
	inline constexpr Vector2& operator/=(float times) noexcept;
	inline constexpr bool operator==(const Vector2& opr) const noexcept;
	inline constexpr bool operator!=(const Vector2& opr) const noexcept;

	/// <summary>
	/// 便利関数
	/// </summary>
public:
	/// <summary>
	/// ベクトルの長さを出す
	/// </summary>
	/// <returns></returns>
	float length(void) const noexcept;

	/// <summary>
	/// ベクトルの正規化
	/// </summary>
	/// <returns></returns>
	const Vector2 normalize(void) const noexcept(false);

	/// <summary>
	/// ベクトルの正規化
	/// </summary>
	/// <param name="tolerance">lengthの許容値</param>
	/// <param name="disapproval">範囲外だった場合の戻り値</param>
	/// <returns></returns>
	const Vector2 normalize_safe(float tolerance = 0.0001f, const Vector2& disapproval = { 1,0 }) const noexcept;

	/// <summary>
	/// Vector3に変換する
	/// </summary>
	/// <param name="z">変換後のzパラメータ値</param>
	/// <returns>変換後のVector3</returns>
	constexpr const Vector3 convert(float z) const noexcept;

	/// <summary>
	/// 静的メンバ関数
	/// </summary>
public:
	/// <summary>
	/// 2つのベクトルの内積を計算
	/// <param name="input1">2次元ベクトル1</param>
	/// <param name="input2">2次元ベクトル2</param>
	/// </summary>
	static constexpr float DotProduct(const Vector2& input1, const Vector2& input2) noexcept;

	/// <summary>
	/// 2つのベクトルの外積を計算
	/// <param name="input1">2次元ベクトル1</param>
	/// <param name="input2">2次元ベクトル2</param>
	/// </summary>
	static constexpr float CrossProduct(const Vector2& input1, const Vector2& input2) noexcept;

	/// <summary>
	/// ベクトルの大きさを計算
	/// <param name="vector">終点の座標</param>
	/// </summary>
	static float Length(const Vector2& vector) noexcept;

	/// <summary>
	/// ベクトルの大きさを計算
	/// <param name="vector1">終点の座標</param>
	/// <param name="vector2">始点の座標</param>
	/// </summary>
	static float Length(const Vector2& vector1, const Vector2& vector2) noexcept;

	/// <summary>
	/// ベクトルの正規化
	/// <param name="vector1">終点の座標</param>
	/// </summary>
	static const Vector2 Normalize(const Vector2& vector);

	/// <summary>
	/// ベクトルの正規化
	/// <param name="vector1">始点の座標</param>
	/// <param name="vector2">終点の座標</param>
	/// </summary>
	static const Vector2 Normalize(const Vector2& vectorFrom, const Vector2& vectorTo);

	/// <summary>
	/// 2つのベクトルを加算
	/// <param name="vector1">足されるベクトル</param>
	/// <param name="vector2">足すベクトル</param>
	/// </summary>
	static constexpr const Vector2 Add(const Vector2& vector1, const Vector2& vector2) noexcept;

	/// <summary>
	/// 2つのベクトルを減算
	/// <param name="vector1">引かれるベクトル</param>
	/// <param name="vector2">引くベクトル</param>
	/// </summary>
	static constexpr const Vector2 Subtract(const Vector2& vector1, const Vector2& vector2) noexcept;

	/// <summary>
	/// ベクトルを定数倍
	/// <param name="vector1">乗算されるベクトル</param>
	/// <param name="times">係数</param>
	/// </summary>
	static constexpr const Vector2 Multiply(const Vector2& vector, const float& times) noexcept;

	/// <summary>
	/// ベクトルを定数倍
	/// <param name="vector1">乗算されるベクトル</param>
	/// <param name="times">係数</param>
	/// </summary>
	static constexpr const Vector2 Multiply(const Vector2& vector, const Vector2& times) noexcept;

	/// <summary>
	/// 2つの位置ベクトルの間の座標
	/// <param name="from">始点の位置ベクトル</param>
	/// <param name="to">終点の位置ベクトル</param>
	/// <param name="t">割合T</param>
	/// </summary>
	static constexpr const Vector2 Lerp(const Vector2& from, const Vector2& to, const float& t) noexcept;

	/// <summary>
	/// <param name="vector">変換するベクトル</param>
	/// <param name="radian">回転量(ラジアン)</param>
	/// </summary>
	static const Vector2 Rotate(const Vector2& vector, const float& radian) noexcept;

	/// <summary>
	/// <param name="vector">変換するベクトル</param>
	/// <param name="radian">回転量(ラジアン)</param>
	/// </summary>
	static constexpr const Vector2 Rotate(const Vector2& vector, const float sinTheta, const float cosTheta) noexcept;

	/// <summary>
	/// 2次ベジエ曲線
	/// </summary>
	/// <param name="initial">始点</param>
	/// <param name="control">制御点</param>
	/// <param name="terminal">終点</param>
	/// <param name="t">媒介変数</param>
	/// <returns></returns>
	static constexpr const Vector2 Bezier(const Vector2& initial, const Vector2& control, const Vector2& terminal, const float t) noexcept;

	/// <summary>
	/// ベクトルの各要素の絶対値化
	/// </summary>
	/// <param name="vector">変化させるベクトル</param>
	/// <returns></returns>
	static const Vector2 Abs(const Vector2& vector) noexcept;

};

inline constexpr Vector2::Vector2() noexcept :
	x(0.0f), y(0.0f) {
}

inline constexpr Vector2::Vector2(float x, float y) noexcept :
	x(x), y(y) {
}

// ------------------インライン関数定義------------------

inline constexpr const Vector2 Vector2::operator+() const noexcept {
	return *this;
}

inline constexpr const Vector2 Vector2::operator-() const noexcept {
	return { -x, -y };
}

inline constexpr const Vector2 Vector2::operator+(const Vector2& opr) const noexcept {
	return { x + opr.x, y + opr.y };
}

inline constexpr const Vector2 Vector2::operator-(const Vector2& opr) const noexcept {
	return { x - opr.x, y - opr.y };
}

inline constexpr const Vector2 Vector2::operator*(float times) const noexcept {
	return { x * times, y * times };
}

inline constexpr const Vector2 Vector2::operator/(float times) const noexcept {
	return { x / times, y / times };
}

inline constexpr Vector2& Vector2::operator+=(const Vector2& opr) noexcept {
	x += opr.x;
	y += opr.y;
	return *this;
}

inline constexpr Vector2& Vector2::operator-=(const Vector2& opr) noexcept {
	x -= opr.x;
	y -= opr.y;
	return *this;
}

inline constexpr Vector2& Vector2::operator*=(float times) noexcept {
	x *= times;
	y *= times;
	return *this;
}

inline constexpr Vector2& Vector2::operator/=(float times) noexcept {
	x /= times;
	y /= times;
	return *this;
}

inline constexpr bool Vector2::operator==(const Vector2& opr) const noexcept {
	return x == opr.x && y == opr.y;
}

inline constexpr bool Vector2::operator!=(const Vector2& opr) const noexcept {
	return !(*this == opr);
}

constexpr const Vector3 Vector2::convert(float z) const noexcept {
	return Vector3{ x,y,z };
}

constexpr float Vector2::DotProduct(const Vector2& input1, const Vector2& input2) noexcept {
	return input1.x * input2.x + input1.y * input2.y;
}

constexpr float Vector2::CrossProduct(const Vector2& input1, const Vector2& input2) noexcept {
	return input1.x * input2.y - input1.y * input2.x;
}

constexpr const Vector2 Vector2::Add(const Vector2& vector1, const Vector2& vector2) noexcept {
	return vector1 + vector2;
}

constexpr const Vector2 Vector2::Subtract(const Vector2& vector1, const Vector2& vector2) noexcept {
	return vector1 - vector2;
}

constexpr const Vector2 Vector2::Multiply(const Vector2& vector, const float& times) noexcept {
	return Vector2{ vector.x * times, vector.y * times };
}

constexpr const Vector2 Vector2::Multiply(const Vector2& vector, const Vector2& times) noexcept {
	return Vector2{ vector.x * times.x, vector.y * times.y };
}

constexpr const Vector2 Vector2::Lerp(const Vector2& from, const Vector2& to, const float& t) noexcept {
	return from * (1 - t) + to * t;
}

constexpr const Vector2 Vector2::Bezier(const Vector2& initial, const Vector2& control, const Vector2& terminal, const float t) noexcept {
	return Lerp(Lerp(initial, control, t), Lerp(control, terminal, t), t);
}

constexpr const Vector2 Vector2::Rotate(const Vector2& vector, const float sinTheta, const float cosTheta) noexcept {
	return Vector2{
		vector.x * cosTheta - vector.y * sinTheta,
		vector.x * sinTheta + vector.y * cosTheta };
}

/// <summary>
/// Vector2定数
/// </summary>
namespace CVector2 {
	constexpr Vector2 BASIS_X = Vector2{ 1.0f, 0.0f }; // x(1.0f), y(0.0f)
	constexpr Vector2 BASIS_Y = Vector2{ 0.0f, 1.0f }; // x(0.0f), y(1.0f)
	constexpr Vector2 ZERO = Vector2{ 0.0f, 0.0f }; // x(0.0f), y(0.0f)
	constexpr Vector2 BASIS = Vector2{ 1.0f, 1.0f }; // x(1.0f), y(1.0f)
	constexpr Vector2 INFINTY = Vector2{ std::numeric_limits<float>::infinity(),std::numeric_limits<float>::infinity() };
	constexpr Vector2 INFINTY_X = Vector2{ std::numeric_limits<float>::infinity(),0 };
	constexpr Vector2 INFINTY_Y = Vector2{ 0, std::numeric_limits<float>::infinity() };
};