#pragma once

#include <optional>
#include <string>

#include <Library/Math/Color4.h>
#include <Library/Math/Vector2.h>
#include <Library/Utility/template/SingletonInterface.h>

class ProjectSettings final : public SingletonInterface<ProjectSettings> {
	__CLASS_SINGLETON_INTERFACE(ProjectSettings)

public:
	struct Application {
		u32 clientWidth{ 1280 };
		u32 clientHeight{ 720 };
		bool isFixWorldClock{ false };
		std::optional<u32> maxFrameRate;
	};

	struct Graphics {
		u32 numBuffering{ 2 };
		std::pair<u32, u32> shaderVersion{ 6, 6 };
		u32 srvHeapSize{ 65536 };
		u32 rtvHeapSize{ 20 };
		u32 dsvHeapSize{ 1 };
		Color4 clearColor{ 0.39f, 0.58f, 0.92f, 1.0f };
	};

public:
	static void Initialize();

	static const std::string& GetProjectName();
	static u32 WindowStyle();

	static const Application& GetApplicationSettings() noexcept;
	static const Graphics& GetGraphicsSettings() noexcept;

	static Vector2 ClientSize() noexcept;
	static u32 ClientWidth() noexcept;
	static u32 ClientHeight() noexcept;

	static void SetMaxFrameRate(std::optional<u32> maxFrameRate) noexcept;

private:
	std::string projectName;
	u32 windowStyle{ 0 };

	Application applicationSettings;
	Graphics graphicsSettings;
};
