#pragma once

#include <array>
#include <filesystem>
#include <format>

#include <Library/Utility/Tools/ChronoUtility.h>

namespace EngineSettings {
// ErrorOBJ/PNG VBVIBV/gpuHandle

// UseRenderingEngine
#define __USE_RENDERING_ENGINE_DIRECTX12
//#define __USE_RENDERING_ENGINE_VULKAN

// FixDeltaSeconds
static constexpr r32 FixDeltaSeconds{ 1.0f / 60.0f };

extern inline bool IsFixDeltaTime{ false };
extern inline bool IsUnlimitedFPS{ false };

extern inline const std::filesystem::path LogFileName{
	std::format(L"{:%F-%H%M%S}.log", ChronoUtility::NowLocalSecond())
};

extern inline const std::filesystem::path LogFilePath{
	std::format(L"./Log/{}", EngineSettings::LogFileName.native())
};

#ifdef DEBUG_FEATURES_ENABLE
static constexpr std::array<u8, 6> LoggerConfigs = {
	0b00000111, // Trace
	0b00000111, // Information
	0b00000111, // Warning
	0b00111111, // Error
	0b00111111, // Critical
	0b00101111, // Assert
};
#else
static constexpr std::array<u8, 6> LoggerConfigs = {
	0b00000000, // Trace
	0b00000000, // Information
	0b00000111, // Warning
	0b00111111, // Error
	0b00111111, // Critical
	0b00000000, // Assert
};
#endif // DEBUG_FEATURES_ENABLE
};
