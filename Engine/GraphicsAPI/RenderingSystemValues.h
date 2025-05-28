#pragma once

#include <memory>

#include <Library/Math/Color4.h>
#include <Library/Utility/Template/Reference.h>

#include "./DirectX/DxResource/TextureResource/DepthStencilTexture.h"

class RenderingSystemValues {
public:
	// バッファリング数
	static inline u32 NUM_BUFFERING{ 0 };

	// DefaultClearColor
	//static constexpr Color4 DEFAULT_CLEAR_COLOR{ .1f, 0.25f, 0.5f, 1.0f };
	static constexpr Color4 DEFAULT_CLEAR_COLOR{ CColor4::BLACK };

private:
	inline static u32 nowBackbufferIndex{ 0 };
	inline static std::unique_ptr<DepthStencilTexture> texture;

public:
	static u32 NowBackbufferIndex() { return nowBackbufferIndex; };
	static void ChangeBackbufferIndex(u32 value) { nowBackbufferIndex = value; };

public:
	static Reference<DepthStencilTexture> GetDepthStencilTexture();

	static void Initialize();
	static void Finalize();
};
