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

void RenderPathManager::UnregisterPath(std::string&& name) {
	// DefaultPathを削除すると色々まずいのでさせない
	if (name == "Default") {
		Log("[RenderPathManager] Don't Unregister path \'Default\'.");
		return;
	}
	auto&& instance = GetInstance();
	RenderPath* deletePath = nullptr;
	// 登録されているか
	if (instance.renderingPath.contains(name)) {
		// 正常に削除されるのでログを出す
		deletePath = &instance.renderingPath.at(name);
		Log(std::format("[RenderPathManager] Unregisterd path. Name-\'{}\' Address-\'{}\'\n", name, (void*)deletePath));
	}
	// 削除
	instance.renderingPath.erase(name);
	// 削除したpathが今のPathと同じだった場合、delete済みポインタ参照が発生する場合があるので、強制的にDefaultにさせる
	if (deletePath == instance.nowPath) {
		SetPath("Default");
		Log(std::format("[RenderPathManager] The path used has been unregistered. Current path is set to \'Default\'\n"));
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
