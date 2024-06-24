#include "RenderPathManager.h"

#include <format>

#include "Engine/Render/RenderPath/RenderPath.h"
#include "Engine/Utility/Utility.h"

#include "Engine/DirectX/DirectXSwapChain/DirectXSwapChain.h"
#include "Engine/Render/RenderNode/Object3DNode/Object3DNode.h"
#include "Engine/Render/RenderTargetGroup/SwapChainRenderTargetGroup.h"
#include "Engine/Render/RenderTargetGroup/SingleRenderTarget.h"

RenderPathManager::RenderPathManager() = default;

RenderPathManager::~RenderPathManager() noexcept = default;

RenderPathManager& RenderPathManager::GetInstance() {
	static RenderPathManager instance;
	return instance;
}

void RenderPathManager::Initialize() {
	auto&& instance = GetInstance();
	instance.create_default();
	SetPath("Default");
}

void RenderPathManager::RegisterPath(std::string&& name, RenderPath&& path) {
	if (!GetInstance().renderingPath.contains(name)) {
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

bool RenderPathManager::IsEnd() {
	return GetInstance().nowPath->is_end();
}

void RenderPathManager::create_default() {
	node = std::make_shared<Object3DNode>();
	node->initialize();
	node->set_render_target_SC(DirectXSwapChain::GetRenderTarget());

	RenderPath path{};
	path.initialize({ node });
	renderingPath.emplace("Default", std::move(path));
}
