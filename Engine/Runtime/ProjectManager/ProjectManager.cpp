#include "ProjectManager.h"

#include <fstream>

#include <json.hpp>

#include "Engine/Application/Output.h"
#include "Engine/Application/EngineSettings.h"
#include "Engine/GraphicsAPI/RenderingSystemValues.h"
#include "Engine/GraphicsAPI/DirectX/DxSystemValues.h"

void ProjectManager::Initialize() {
	ProjectManager& instance = GetInstance();

	instance.graphicsSettings = {
		2,
		{6,6},
		65536,
		20,
		1,
	};

	nlohmann::json json{};

	std::ifstream ifstream{ "./Game/Assets/Core/Core.json" };

	if (ifstream.fail()) {
		Error("Project is not found.");
		return;
	}

	json = nlohmann::json::parse(ifstream);

	instance.projectName = json["ProjectName"].get<std::string>();
	instance.windowConfig = json["WindowConfig"].get<u32>();
	if (json.contains("Graphics")) {
		nlohmann::json graphics = json["Graphics"];
		if (graphics.contains("NumBuffering")) {
			instance.graphicsSettings.numBuffering = graphics["NumBuffering"].get<u32>();
		}
		if (graphics.contains("ShaderVersion")) {
			std::string ver = graphics["ShaderVersion"].get<std::string>();
			instance.graphicsSettings.shaderVersion = {
				ver[0] - '0',ver[2] - '0'
			};
		}
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

	if (json.contains("IsFixWorldClock")) {
		EngineSettings::IsFixDeltaTime = json["IsFixWorldClock"];
	}

	RenderingSystemValues::NUM_BUFFERING = instance.graphicsSettings.numBuffering;
	
	DxSystemValues::HEAP_SIZE_SRV = instance.graphicsSettings.srvHeapSize;
	DxSystemValues::HEAP_SIZE_RTV = instance.graphicsSettings.rtvHeapSize;
	DxSystemValues::HEAP_SIZE_DSV = instance.graphicsSettings.dsvHeapSize;

	DxSystemValues::SHADER_VERSION = instance.graphicsSettings.shaderVersion;
}

const std::string& ProjectManager::GetProjectName() {
	return GetInstance().projectName;
}

u32 ProjectManager::WindowConfig() {
	return GetInstance().windowConfig;
}
