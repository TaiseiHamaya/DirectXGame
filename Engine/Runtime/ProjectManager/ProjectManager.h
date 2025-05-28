#pragma once

#include <string>

#include <Library/Utility/template/SingletonInterface.h>

class ProjectManager final : public SingletonInterface<ProjectManager> {
	__CLASS_SINGLETON_INTERFACE(ProjectManager)

public:
	static void Initialize();

	static const std::string& GetProjectName();
	static u32 WindowConfig();

private:
	std::string projectName;
	u32 windowConfig{ 0 };

	struct Graphics {
		u32 numBuffering;
		std::pair<u32, u32> shaderVersion;
		u32 srvHeapSize;
		u32 rtvHeapSize;
		u32 dsvHeapSize;
	} graphicsSettings;
};
