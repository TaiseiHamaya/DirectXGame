#pragma once

#include <string>

#include <Library/Utility/template/SingletonInterface.h>
#include <Library/Math/Vector2.h>

class ProjectSettings final : public SingletonInterface<ProjectSettings> {
	__CLASS_SINGLETON_INTERFACE(ProjectSettings)

public:
	static void Initialize();

	static const std::string& GetProjectName();
	static u32 WindowStyle();

	static Vector2 ClientSize() noexcept;
	static u32 ClientWidth() noexcept;
	static u32 ClientHeight() noexcept;

private:
	std::string projectName;
	u32 windowStyle{ 0 };

	struct Graphics {
		u32 numBuffering;
		std::pair<u32, u32> shaderVersion;
		u32 srvHeapSize;
		u32 rtvHeapSize;
		u32 dsvHeapSize;
	} graphicsSettings;

	u32 clientWidth{ 1280 };
	u32 clientHeight{ 720 };
};
