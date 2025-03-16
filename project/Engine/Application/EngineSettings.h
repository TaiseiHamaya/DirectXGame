#pragma once

#include <cstdint>
#include <string_view>

#include <Library/Math/Vector2.h>

namespace EngineSettings {
// Windowタイトル
// Framework作ったらそっちに移行する？
static constexpr std::wstring_view WINDOW_TITLE_W{ L"DirectXGame\0" };

// DefaultWindowSetting
//static constexpr DWORD WindowConfig

// WindowSize
static constexpr Vector2 CLIENT_SIZE{ 1280, 720 };
static constexpr std::uint32_t  CLIENT_WIDTH{ static_cast<std::uint32_t>(CLIENT_SIZE.x) };
static constexpr std::uint32_t  CLIENT_HEIGHT{ static_cast<std::uint32_t>(CLIENT_SIZE.y) };

// ErrorOBJ/PNG VBVIBV/gpuHandle

// UseRenderingEngine
#define __USE_RENDERING_ENGINE_DIRECTX12
//#define __USE_RENDERING_ENGINE_VULKAN

	// FixDeltaSeconds
static constexpr float FixDeltaSeconds{ 1.0f / 60.0f };

extern inline bool IsFixDeltaTime{ false };
extern inline bool IsUnlimitedFPS{ false };

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
#ifdef DEBUG_FEATURES_ENABLE //                                  C     E     W     I    
static constexpr uint32_t LogOutputConfigFlags{ 0b111111111111111000111000 };
#else //                                          C     E     W     I    
static constexpr uint32_t LogOutputConfigFlags{ 0b110111110111110000000000 };
#endif // _DEBUG
};
