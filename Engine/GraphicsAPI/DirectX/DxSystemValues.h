#pragma once

#include <dxgiformat.h>

namespace DxSystemValues {
// HeapSize
extern inline u32  HEAP_SIZE_SRV = 65536;
extern inline u32  HEAP_SIZE_RTV = 10;
extern inline u32  HEAP_SIZE_DSV = 1;

// ScreenRTVFormat
static constexpr DXGI_FORMAT SWAPCHAIN_FORMAT = DXGI_FORMAT_R8G8B8A8_UNORM;
static constexpr DXGI_FORMAT SCREEN_RTV_FORMAT = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

// Anisotropy
static constexpr u32 ANISOTROPY{ 16 };

// ShaderVersion
extern inline std::pair<u32, u32> SHADER_VERSION{ 6, 6 };
};
