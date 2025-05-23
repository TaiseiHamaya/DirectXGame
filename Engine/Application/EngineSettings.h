#pragma once

#include <filesystem>
#include <format>

#include <Library/Math/Vector2.h>
#include <Library/Utility/Tools/ChronoUtility.h>

namespace EngineSettings {
// WindowSize
static constexpr Vector2 CLIENT_SIZE{ 1280, 720 };
static constexpr u32 CLIENT_WIDTH{ static_cast<u32>(CLIENT_SIZE.x) };
static constexpr u32 CLIENT_HEIGHT{ static_cast<u32>(CLIENT_SIZE.y) };

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

// メモ
// 上位ビットから6bitずつCritical,Error,Warning,Infomation
// 各6bitの割り当ては上から
//    コンソール出力
//    ファイル出力
//    Editor出力(未実装)
//    ウィンドウ出力
//    ブレークポイント命令
//    Stacktrace出力
//
// つまり
// [CriticalConfig6bit][ErrorConfig6bit][WarningConfig6bit][InfoConfig6bit]
// の24bit
#ifdef DEBUG_FEATURES_ENABLE
//                                                C     E     W     I    
static constexpr u32 LogOutputConfigFlags{ 0b111111111111111000111000 };
#else
//                                                C     E     W     I    
static constexpr u32 LogOutputConfigFlags{ 0b110111110101110000000000 };
#endif // DEBUG_FEATURES_ENABLE
};
