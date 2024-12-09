#pragma once

#include <cstdint>

class Color3 final {
public:
	/// <summary>
	/// デフォルトコンストラクタ(1,1,1,1で初期化)
	/// </summary>
	constexpr Color3() noexcept = default;

	/// <summary>
	/// カラーコードから生成
	/// </summary>
	/// <param name="hex">カラーコード</param>
	constexpr Color3(std::uint32_t hex) noexcept;

	/// <summary>
	/// RGB別カラーコードから生成
	/// </summary>
	/// <param name="_red">赤[0x00, 0xff]</param>
	/// <param name="_green">緑[0x00, 0xff]</param>
	/// <param name="_blue">青[0x00, 0xff]</param>
	/// <param name="_alpha">アルファ[0x00, 0xff]</param>
	constexpr Color3(std::uint8_t _red, std::uint8_t _green, std::uint8_t _blue) noexcept;

	/// <summary>
	/// [0,1]のカラーデータから生成
	/// </summary>
	/// <param name="_red">赤[0, 1]</param>
	/// <param name="_green">緑[0, 1]</param>
	/// <param name="_blue">青[0, 1]</param>
	/// <param name="_alpha">アルファ[0, 1]</param>
	constexpr Color3(float _red, float _green, float _blue) noexcept;

public:
	float red{ 1.0f }; // 赤[0, 1]
	float green{ 1.0f }; // 緑[0, 1]
	float blue{ 1.0f }; // 青[0, 1]

#ifdef _DEBUG
public:
	void debug_gui(const char* tag = "Color3") noexcept(false);
#endif // _DEBUG
};

constexpr Color3::Color3(std::uint32_t hex) noexcept :
	red(((hex >> 24) & 0x000000ff) / 255.0f),
	green(((hex >> 16) & 0x000000ff) / 255.0f),
	blue(((hex >> 8) & 0x000000ff) / 255.0f) {
}

constexpr Color3::Color3(std::uint8_t _red, std::uint8_t _green, std::uint8_t _blue) noexcept :
	red(_red / 255.0f),
	green(_green / 255.0f),
	blue(_blue / 255.0f) {
}

constexpr Color3::Color3(float _red, float _green, float _blue) noexcept :
	red(_red),
	green(_green),
	blue(_blue) {
}

/// <summary>
/// Color定数
/// </summary>
namespace CColor3 {

constexpr Color3 WHITE{ 1.0f, 1.0f, 1.0f };
constexpr Color3 BLACK{ 0.0f, 0.0f, 0.0f };
constexpr Color3 RED{ 1.0f, 0.0f, 0.0f };
constexpr Color3 GREEN{ 0.0f, 1.0f, 0.0f };
constexpr Color3 BLUE{ 0.0f, 0.0f, 1.0f };

constexpr Color3 YELLOW{ 1.0f, 1.0f, 0.0f };
constexpr Color3 MAGENTA{ 1.0f, 0.0f, 1.0f };
constexpr Color3 CYAN{ 0.0f, 1.0f, 1.0f };

constexpr Color3 ZERO{ 0.0f, 0.0f, 0.0f };
constexpr Color3 ZERO_WHITE{ 1.0f, 1.0f, 1.0f };

}


