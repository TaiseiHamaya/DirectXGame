#pragma once

#include <dxgiformat.h>

namespace DxSystemValues {
// ScreenRTVFormat
static constexpr DXGI_FORMAT SWAPCHAIN_FORMAT = DXGI_FORMAT_R8G8B8A8_UNORM;
static constexpr DXGI_FORMAT SCREEN_RTV_FORMAT = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

// Anisotropy
static constexpr u32 ANISOTROPY{ 16 };
};
