#include "ProjectSettings.h"

#include <fstream>

#include <json.hpp>

#include "Engine/Application/EngineSettings.h"
#include "Engine/Application/Output.h"
#include "Engine/GraphicsAPI/DirectX/DxSystemValues.h"
#include "Engine/GraphicsAPI/RenderingSystemValues.h"

constexpr u32 DEFAULT_WINDOW_STYLE = WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME;

void ProjectSettings::Initialize() {
	ProjectSettings& instance = GetInstance();

	instance.graphicsSettings = {
		2,
		{6,6},
		65536,
		20,
		1,
	};

	nlohmann::json json{};

	std::ifstream ifstream{ "./Game/Core/Core.json" };

	if (ifstream.fail()) {
		Error("Project is not found.");
		return;
	}

	json = nlohmann::json::parse(ifstream);

	instance.projectName = json["ProjectName"].get<std::string>();
	if (json.contains("WindowStyle")) {
		nlohmann::json& jsonWindowStyle = json["WindowStyle"];
		instance.windowStyle = 0;
		bool isFullscreen = false;
		// フルスクリーン
		if (jsonWindowStyle.value("Fullscreen", false)) {
			instance.windowStyle = WS_POPUP;
			isFullscreen = true;
		}
		else {
			instance.windowStyle = DEFAULT_WINDOW_STYLE;
		}
		if (!isFullscreen) {
			// 最大化ボタン
			if (jsonWindowStyle.value("MaximizeBox", false)) {
				instance.windowStyle |= WS_MAXIMIZEBOX;

			}
			else {
				instance.windowStyle &= ~WS_MAXIMIZEBOX;
			}

			// 最小化ボタン
			if (jsonWindowStyle.value("MinimizeBox", true)) {
				instance.windowStyle |= WS_MINIMIZEBOX;
			}
			else {
				instance.windowStyle &= ~WS_MINIMIZEBOX;
			}

			// サイズ変更可能
			if (jsonWindowStyle.value("Resizable", false)) {
				instance.windowStyle |= WS_THICKFRAME;
			}
			else {
				instance.windowStyle &= ~WS_THICKFRAME;
			}

			// 最大化状態で起動
			if (jsonWindowStyle.value("MaximizeDefault", false)) {
				instance.windowStyle |= WS_MAXIMIZE;
			}
			else {
				instance.windowStyle &= ~WS_MAXIMIZE;
			}
		}
	}
	
	// DirectX設定
	if (json.contains("Graphics")) {
		nlohmann::json& graphics = json["Graphics"];
		// バッファリング数
		if (graphics.contains("NumBuffering")) {
			instance.graphicsSettings.numBuffering = graphics["NumBuffering"].get<u32>();
		}
		// シェーダーバージョン
		if (graphics.contains("ShaderVersion")) {
			std::string ver = graphics["ShaderVersion"].get<std::string>();
			instance.graphicsSettings.shaderVersion = {
				ver[0] - '0',ver[2] - '0'
			};
		}
		// DescriptorHeapの数
		if (graphics.contains("SrvHeapSize")) {
			instance.graphicsSettings.srvHeapSize = graphics["SrvHeapSize"].get<u32>();
		}
		if (graphics.contains("RtvHeapSize")) {
			instance.graphicsSettings.rtvHeapSize = graphics["RtvHeapSize"].get<u32>();
		}
		if (graphics.contains("DsvHeapSize")) {
			instance.graphicsSettings.dsvHeapSize = graphics["DsvHeapSize"].get<u32>();
		}
	}
	
	// デフォルトのウィンドウサイズ
	if (json.contains("WindowSize")) {
		nlohmann::json& windowSize = json["WindowSize"];
		if (windowSize.contains("Width")) {
			instance.clientWidth = windowSize["Width"].get<u32>();
		}
		if (windowSize.contains("Height")) {
			instance.clientHeight = windowSize["Height"].get<u32>();
		}
	}

	// WorldClockを修正するかどうか
	if (json.contains("IsFixWorldClock")) {
		EngineSettings::IsFixDeltaTime = json["IsFixWorldClock"];
	}

	RenderingSystemValues::NUM_BUFFERING = instance.graphicsSettings.numBuffering;

	DxSystemValues::HEAP_SIZE_SRV = instance.graphicsSettings.srvHeapSize;
	DxSystemValues::HEAP_SIZE_RTV = instance.graphicsSettings.rtvHeapSize;
	DxSystemValues::HEAP_SIZE_DSV = instance.graphicsSettings.dsvHeapSize;

	DxSystemValues::SHADER_VERSION = instance.graphicsSettings.shaderVersion;
}

const std::string& ProjectSettings::GetProjectName() {
	return GetInstance().projectName;
}

u32 ProjectSettings::WindowStyle() {
	return GetInstance().windowStyle;
}

Vector2 ProjectSettings::ClientSize() noexcept {
	auto& instance = GetInstance();
	return Vector2{
		static_cast<r32>(instance.clientWidth),
		static_cast<r32>(instance.clientHeight)
	};
}

u32 ProjectSettings::ClientWidth() noexcept {
	return static_cast<u32>(GetInstance().clientWidth);
}

u32 ProjectSettings::ClientHeight() noexcept {
	return static_cast<u32>(GetInstance().clientHeight);
}
