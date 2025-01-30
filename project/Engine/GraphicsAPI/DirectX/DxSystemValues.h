#pragma once

#include <cstdint>
#include <string_view>

#include <dxgiformat.h>
#include <minwindef.h>

namespace DxSystemValues {
// HeapSize
static constexpr std::uint32_t  HEAP_SIZE_SRV = 65536;
static constexpr std::uint32_t  HEAP_SIZE_RTV = 10;
static constexpr std::uint32_t  HEAP_SIZE_DSV = 1;

// ScreenRTVFormat
static constexpr DXGI_FORMAT SWAPCHAIN_FORMAT = DXGI_FORMAT_R8G8B8A8_UNORM;
static constexpr DXGI_FORMAT SCREEN_RTV_FORMAT = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

// NumDirectXCommand
// TODO : Texture用Command関連を分離したら定義

// Anisotropy
static constexpr UINT ANISOTROPY{ 16 };

// ShaderVersion
// 使い方がキモいので修正したほうが良いかも
static constexpr std::wstring_view SHADER_VERSION{ L"6_1" };
//#define SHADER_VERSION L"6_1"
};