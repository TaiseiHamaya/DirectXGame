#pragma once

#include <dxgiformat.h>

namespace DxSystemValues {
// HeapSize
static constexpr u32  HEAP_SIZE_SRV = 65536;
static constexpr u32  HEAP_SIZE_RTV = 10;
static constexpr u32  HEAP_SIZE_DSV = 1;

// ScreenRTVFormat
static constexpr DXGI_FORMAT SWAPCHAIN_FORMAT = DXGI_FORMAT_R8G8B8A8_UNORM;
static constexpr DXGI_FORMAT SCREEN_RTV_FORMAT = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

// NumDirectXCommand
// TODO : Texture用Command関連を分離したら定義

// Anisotropy
static constexpr UINT ANISOTROPY{ 16 };

// ShaderVersion
static constexpr std::pair<u32, u32> SHADER_VERSION{ 6, 6 };
//#define SHADER_VERSION L"6_1"
};
