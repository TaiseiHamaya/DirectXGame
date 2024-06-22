#include "RenderPathManager.h"

#include <format>

#include "Engine/DirectX/PipelineState/PipelineState.h"
#include "Engine/Render/RenderPath/RenderPath.h"
#include "Engine/DirectX/DirectXCore.h"
#include "Engine/Utility/Utility.h"

RenderPathManager& RenderPathManager::GetInstance() {
	static RenderPathManager instance;
	return instance;
}

void RenderPathManager::Initialize() {
	auto&& instance = GetInstance();
	instance.renderingPath.emplace("Default", RenderPath{});
	SetPath("Default");
}

void RenderPathManager::RegisterPath(std::string&& name, RenderPath&& path) {
	if (GetInstance().renderingPath.contains(name)) {
		GetInstance().renderingPath.emplace(std::move(name), std::move(path));
	}
	else {
		Log(std::format("[RenderPathManager] Registering aleady used name. Name-\'{}\'", name));
	}
}

void RenderPathManager::SetPath(const std::string& name) {
	GetInstance().nowPath = &GetInstance().renderingPath.at(name);
}

bool RenderPathManager::BeginFrame() {
	return GetInstance().nowPath->begin();
}

bool RenderPathManager::Next() {
	return GetInstance().nowPath->next();
}
