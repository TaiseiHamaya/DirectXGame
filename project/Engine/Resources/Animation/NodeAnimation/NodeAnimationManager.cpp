#include "NodeAnimationManager.h"

#include <mutex>

#include "Engine/Debug/Output.h"
#include "Engine/Resources/BackgroundLoader/BackgroundLoader.h"
#include "NodeAnimationResource.h"

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

std::mutex nodeAnimationMutex;

void NodeAnimationManager::RegisterLoadQue(const std::filesystem::path& filePath) {
	// ロード済みの場合は何もしない
	if (IsRegistered(filePath.filename().string())) {
		return;
	}
	// BackgroundLoaderにLoadPolygonMeshイベントとして転送
	//BackgroundLoader::RegisterLoadQue(LoadEvent::, filePath);
}

std::shared_ptr<const NodeAnimationResource> NodeAnimationManager::GetAnimation(const std::string& name) {
	std::lock_guard<std::mutex> lock{ nodeAnimationMutex };
	if (IsRegisteredNonlocking(name)) {
		return GetInstance().instanceList.at(name);
	}
	else {
		//Console("[NodeAnimationManager] Unloading . Name-\'{:}\'\n", name);
		return nullptr;
	}
}

bool NodeAnimationManager::IsRegistered(const std::string& name) {
	std::lock_guard<std::mutex> lock{ nodeAnimationMutex };
	return IsRegisteredNonlocking(name);
}

void NodeAnimationManager::Transfer(const std::string& name, std::shared_ptr<NodeAnimationResource>& data) {
	std::lock_guard<std::mutex> lock{ nodeAnimationMutex };
	Console("[NodeAnimationManager] Transfer new node animation. Name-\'{:}\', Address-\'{:}\'\n", name, (void*)data.get());
	GetInstance().instanceList.emplace(name, data);
}

bool NodeAnimationManager::IsRegisteredNonlocking(const std::string& name) {
	return GetInstance().instanceList.contains(name);
}
