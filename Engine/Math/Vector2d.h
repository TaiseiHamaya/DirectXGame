#pragma once

#include <utility>

class Vector3;

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
	inline constexpr Vector2() noexcept :
		x(0.0f), y(0.0f) {
	}

	inline constexpr Vector2(float x, float y) noexcept :
		x(x), y(y) {
	}

	inline constexpr Vector2(const Vector2& vec) noexcept :
		x(vec.x), y(vec.y) {
	}

	inline constexpr Vector2(Vector2&& vec) noexcept :
		x(std::move(vec.x)), y(std::move(vec.y)) {
	}

	~Vector2() = default;

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
	inline constexpr Vector2& operator=(const Vector2& vec) noexcept;
	inline constexpr Vector2& operator=(Vector2&& vec) noexcept;
	inline constexpr Vector2 operator+(void) const noexcept;
	inline constexpr Vector2 operator-(void) const noexcept;
	inline constexpr Vector2 operator+(const Vector2& opr) const noexcept;
	inline constexpr Vector2 operator-(const Vector2& opr) const noexcept;
	inline constexpr Vector2 operator*(float times) const noexcept;
	inline constexpr Vector2 operator/(float times) const;
	inline Vector2& operator+=(const Vector2& opr) noexcept;
	inline Vector2& operator-=(const Vector2& opr) noexcept;
	inline Vector2& operator*=(float times) noexcept;
	inline Vector2& operator/=(float times);
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
	float length(void) const;

	/// <summary>
	/// ベクトルの正規化
	/// </summary>
	/// <returns></returns>
	Vector2 normalize(void) const;

	Vector3 convert_3d(float z) const;

	/// <summary>
	/// 静的メンバ関数
	/// </summary>
public:
	/// <summary>
	/// 2つのベクトルの内積を計算
	/// <param name="input1">2次元ベクトル1</param>
	/// <param name="input2">2次元ベクトル2</param>
	/// </summary>
	static float DotProduct(const Vector2& input1, const Vector2& input2);

	/// <summary>
	/// 2つのベクトルの外積を計算
	/// <param name="input1">2次元ベクトル1</param>
	/// <param name="input2">2次元ベクトル2</param>
	/// </summary>
	static float CrossProduct(const Vector2& input1, const Vector2& input2);

	/// <summary>
	/// ベクトルの大きさを計算
	/// <param name="vector">終点の座標</param>
	/// </summary>
	static float Length(const Vector2& vector);

	/// <summary>
	/// ベクトルの大きさを計算
	/// <param name="vector1">終点の座標</param>
	/// <param name="vector2">始点の座標</param>
	/// </summary>
	static float Length(const Vector2& vector1, const Vector2& vector2);

	/// <summary>
	/// ベクトルの正規化
	/// <param name="vector1">終点の座標</param>
	/// </summary>
	static Vector2 Normalize(const Vector2& vector);

	/// <summary>
	/// ベクトルの正規化
	/// <param name="vector1">始点の座標</param>
	/// <param name="vector2">終点の座標</param>
	/// </summary>
	static Vector2 Normalize(const Vector2& vectorFrom, const Vector2& vectorTo);

	/// <summary>
	/// 2つのベクトルを加算
	/// <param name="vector1">足されるベクトル</param>
	/// <param name="vector2">足すベクトル</param>
	/// </summary>
	static Vector2 Add(const Vector2& vector1, const Vector2& vector2);

	/// <summary>
	/// 2つのベクトルを減算
	/// <param name="vector1">引かれるベクトル</param>
	/// <param name="vector2">引くベクトル</param>
	/// </summary>
	static Vector2 Subtract(const Vector2& vector1, const Vector2& vector2);

	/// <summary>
	/// ベクトルを定数倍
	/// <param name="vector1">乗算されるベクトル</param>
	/// <param name="times">係数</param>
	/// </summary>
	static Vector2 Multiply(const Vector2& vector, const float& times);

	/// <summary>
	/// ベクトルを定数倍
	/// <param name="vector1">乗算されるベクトル</param>
	/// <param name="times">係数</param>
	/// </summary>
	static Vector2 Multiply(const Vector2& vector, const Vector2& times);

	/// <summary>
	/// 2つの位置ベクトルの間の座標
	/// <param name="from">始点の位置ベクトル</param>
	/// <param name="to">終点の位置ベクトル</param>
	/// <param name="t">割合T</param>
	/// </summary>
	static Vector2 Lerp(const Vector2& from, const Vector2& to, const float& t);

	/// <summary>
	/// <param name="vector">変換するベクトル</param>
	/// <param name="radian">回転量(ラジアン)</param>
	/// </summary>
	static Vector2 Rotate(const Vector2& vector, const float& radian);

	/// <summary>
	/// <param name="vector">変換するベクトル</param>
	/// <param name="radian">回転量(ラジアン)</param>
	/// </summary>
	static Vector2 Rotate(const Vector2& vector, const float sintheta, const float costheta);

	/// <summary>
	/// 2次ベジエ曲線
	/// </summary>
	/// <param name="initial">始点</param>
	/// <param name="control">制御点</param>
	/// <param name="terminal">終点</param>
	/// <param name="t">媒介変数</param>
	/// <returns></returns>
	static Vector2 Bezier(const Vector2& initial, const Vector2& control, const Vector2& terminal, const float t);

	/// <summary>
	/// ベクトルの各要素の絶対値化
	/// </summary>
	/// <param name="vector">変化させるベクトル</param>
	/// <returns></returns>
	static Vector2 Abs(const Vector2& vector);

};

/// <summary>
/// Vector2定数
/// </summary>
namespace Vec2{
	constexpr Vector2 kBasisX = Vector2{ 1.0f, 0.0f }; // x(1.0f), y(0.0f)
	constexpr Vector2 kBasisY = Vector2{ 0.0f, 1.0f }; // x(0.0f), y(1.0f)
	constexpr Vector2 kZero = Vector2{ 0.0f, 0.0f }; // x(0.0f), y(0.0f)
	constexpr Vector2 kBasis = Vector2{ 1.0f, 1.0f }; // x(1.0f), y(1.0f)
};

// ------------------インライン関数定義------------------
inline constexpr Vector2& Vector2::operator=(const Vector2& vec) noexcept {
	x = vec.x;
	y = vec.y;
	return *this;
}

inline constexpr Vector2& Vector2::operator=(Vector2&& vec) noexcept {
	x = std::move(vec.x);
	y = std::move(vec.y);
	return *this;
}

inline constexpr Vector2 Vector2::operator+() const noexcept {
	return *this;
}

inline constexpr Vector2 Vector2::operator-() const noexcept {
	return { -x, -y };
}

inline constexpr Vector2 Vector2::operator+(const Vector2& opr) const noexcept {
	return { x + opr.x, y + opr.y };
}

inline constexpr Vector2 Vector2::operator-(const Vector2& opr) const noexcept {
	return { x - opr.x, y - opr.y };
}

inline constexpr Vector2 Vector2::operator*(float times) const noexcept {
	return { x * times, y * times };
}

inline constexpr Vector2 Vector2::operator/(float times) const {
	return { x / times, y / times };
}

inline Vector2& Vector2::operator+=(const Vector2& opr) noexcept {
	x += opr.x;
	y += opr.y;
	return *this;
}

inline Vector2& Vector2::operator-=(const Vector2& opr) noexcept {
	x -= opr.x;
	y -= opr.y;
	return *this;
}

inline Vector2& Vector2::operator*=(float times) noexcept {
	x *= times;
	y *= times;
	return *this;
}

inline Vector2& Vector2::operator/=(float times) {
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