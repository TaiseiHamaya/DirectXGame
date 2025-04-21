#pragma once

#include <cstdint>

#include <Library/Math/Color4.h>

class RenderingSystemValues {
public:
	// バッファリング数
	static constexpr std::uint32_t NUM_BUFFERING{ 2 };

	// DefaultClearColor
	//static constexpr Color4 DEFAULT_CLEAR_COLOR{ .1f, 0.25f, 0.5f, 1.0f };
	static constexpr Color4 DEFAULT_CLEAR_COLOR{ CColor4::BLACK };

private:
	inline static std::uint32_t nowBackbufferIndex{ 0 };

public:
	static std::uint32_t NowBackbufferIndex() { return nowBackbufferIndex; };
	static void ChangeBackbufferIndex(std::uint32_t value) { nowBackbufferIndex = value; };
};
