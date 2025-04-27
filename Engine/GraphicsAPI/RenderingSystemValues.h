#pragma once

#include <Library/Math/Color4.h>

class RenderingSystemValues {
public:
	// バッファリング数
	static constexpr u32 NUM_BUFFERING{ 2 };

	// DefaultClearColor
	//static constexpr Color4 DEFAULT_CLEAR_COLOR{ .1f, 0.25f, 0.5f, 1.0f };
	static constexpr Color4 DEFAULT_CLEAR_COLOR{ CColor4::BLACK };

private:
	inline static u32 nowBackbufferIndex{ 0 };

public:
	static u32 NowBackbufferIndex() { return nowBackbufferIndex; };
	static void ChangeBackbufferIndex(u32 value) { nowBackbufferIndex = value; };
};
