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
};