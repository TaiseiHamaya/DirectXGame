#pragma once

#include <memory>

#include <Library/Utility/Template/Reference.h>

#include "./DirectX/DxResource/TextureResource/DepthStencilTexture.h"

class RenderingSystemValues {
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
