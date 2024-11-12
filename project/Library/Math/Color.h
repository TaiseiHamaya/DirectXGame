#pragma once

#include <cstdint>

class Color final {
public:
	/// <summary>
	/// デフォルトコンストラクタ(1,1,1,1で初期化)
	/// </summary>
	constexpr Color() noexcept = default;

	/// <summary>
	/// カラーコードから生成
	/// </summary>
	/// <param name="hex">カラーコード</param>
	constexpr Color(std::uint32_t hex) noexcept;

	/// <summary>
	/// RGB別カラーコードから生成
	/// </summary>
	/// <param name="_red">赤[0x00, 0xff]</param>
	/// <param name="_green">緑[0x00, 0xff]</param>
	/// <param name="_blue">青[0x00, 0xff]</param>
	/// <param name="_alpha">アルファ[0x00, 0xff]</param>
	constexpr Color(std::uint8_t _red, std::uint8_t _green, std::uint8_t _blue, std::uint8_t _alpha) noexcept;

	/// <summary>
	/// [0,1]のカラーデータから生成
	/// </summary>
	/// <param name="_red">赤[0, 1]</param>
	/// <param name="_green">緑[0, 1]</param>
	/// <param name="_blue">青[0, 1]</param>
	/// <param name="_alpha">アルファ[0, 1]</param>
	constexpr Color(float _red, float _green, float _blue, float _alpha) noexcept;

public:
	float red{ 1.0f }; // 赤[0, 1]
	float green{ 1.0f }; // 緑[0, 1]
	float blue{ 1.0f }; // 青[0, 1]
	float alpha{ 1.0f }; // アルファ[0, 1]

#ifdef _DEBUG
public:
	void debug_gui3() noexcept(false);

	void debug_gui4() noexcept(false);
#endif // _DEBUG

public:
	/// <summary>
	/// 色の線形補間(Easing.hが存在しないため未実装)
	/// </summary>
	/// <param name="color1">t = 0の時の色</param>
	/// <param name="color2">t = 1の時の色</param>
	/// <param name="t">媒介変数</param>
	/// <returns></returns>
	[[deprecated]] static Color LerpC(const Color& color1, const Color& color2, float t);
};

constexpr Color::Color(std::uint32_t hex) noexcept {
	red = ((hex >> 24) & 0x000000ff) / 255.0f;
	green = ((hex >> 16) & 0x000000ff) / 255.0f;
	blue = ((hex >> 8) & 0x000000ff) / 255.0f;
	alpha = ((hex >> 0) & 0x000000ff) / 255.0f;
}

constexpr Color::Color(std::uint8_t _red, std::uint8_t _green, std::uint8_t _blue, std::uint8_t _alpha) noexcept {
	red = _red / 255.0f;
	green = _green / 255.0f;
	blue = _blue / 255.0f;
	alpha = _alpha / 255.0f;
}

constexpr Color::Color(float _red, float _green, float _blue, float _alpha) noexcept {
	red = _red;
	green = _green;
	blue = _blue;
	alpha = _alpha;
}

/// <summary>
/// Color定数
/// </summary>
namespace CColor {

constexpr Color WHITE{ 1.0f, 1.0f, 1.0f, 1.0f };
constexpr Color BLACK{ 0.0f, 0.0f, 0.0f, 1.0f };
constexpr Color RED{ 1.0f, 0.0f, 0.0f, 1.0f };
constexpr Color GREEN{ 0.0f, 1.0f, 0.0f, 1.0f };
constexpr Color BLUE{ 0.0f, 0.0f, 1.0f, 1.0f };

constexpr Color YELLOW{ 1.0f, 1.0f, 0.0f, 1.0f };
constexpr Color MAGENTA{ 1.0f, 0.0f, 1.0f, 1.0f };
constexpr Color CYAN{ 0.0f, 1.0f, 1.0f, 1.0f };

constexpr Color ZERO{ 0.0f, 0.0f, 0.0f, 0.0f };
constexpr Color ZERO_WHITE{ 1.0f, 1.0f, 1.0f, 0.0f };

}
