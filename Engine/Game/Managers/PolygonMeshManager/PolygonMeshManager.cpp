#include "PolygonMeshManager.h"

#include <format>
#include <mutex>

#include "Engine/Game/PolygonMesh/PolygonMesh.h"
#include "Engine/Utility/BackgroundLoader/BackgroundLoader.h"
#include "Engine/Utility/Utility.h"

#ifdef _DEBUG
#include "externals/imgui/imgui.h"
#endif // _DEBUG

std::mutex meshMutex;

PolygonMeshManager::PolygonMeshManager() noexcept = default;

PolygonMeshManager::~PolygonMeshManager() noexcept = default;

PolygonMeshManager& PolygonMeshManager::GetInstance() noexcept {
	static std::unique_ptr<PolygonMeshManager> instance{ new PolygonMeshManager };
	return *instance;
}

void PolygonMeshManager::RegisterLoadQue(const std::string& directoryPath, const std::string& fileName) {
	// ロード済みの場合は何もしない
	if (IsRegistered(fileName)) {
		return;
	}
	// BackgroundLoaderにLoadPolygonMeshイベントとして転送
	BackgroundLoader::RegisterLoadQue(LoadEvent::LoadPolygonMesh, directoryPath, fileName);
}

std::weak_ptr<PolygonMesh> PolygonMeshManager::GetPolygonMesh(const std::string& meshName) {
	std::lock_guard<std::mutex> lock{ meshMutex }; 
	if (IsRegisteredUnlocking(meshName)) {
		return GetInstance().meshInstanceList.at(meshName);
	}
	else {
		// 存在しないメッシュを呼び出したらエラー用メッシュを使用する
		Log(std::format("[PolygonMeshManager] Unloading polygon mesh. Name-\'{:}\'\n", meshName));
		return GetInstance().meshInstanceList.at("ErrorObject.obj");
	}
}

bool PolygonMeshManager::IsRegistered(const std::string& meshName) {
	std::lock_guard<std::mutex> lock{ meshMutex };
	return IsRegisteredUnlocking(meshName);
}

void PolygonMeshManager::Transfer(const std::string& name, std::shared_ptr<PolygonMesh>& data) {
	std::lock_guard<std::mutex> lock{ meshMutex };
	Log(std::format("[PolygonMeshManager] Transfer new PolygonMesh. Name-\'{:}\', Address-\'{:}\'\n", name, (void*)data.get()));
	GetInstance().meshInstanceList.emplace(name, data);
}

#ifdef _DEBUG
bool PolygonMeshManager::MeshListGui(std::string& current) {
	bool changed = false;

	std::lock_guard<std::mutex> lock{ meshMutex };
	if(ImGui::BeginCombo("MeshList", current.c_str())) {
		auto&& list = GetInstance().meshInstanceList;
		for (auto itr = list.begin(); itr != list.end(); ++itr) {
			bool is_selected = (current == itr->first);
			if (ImGui::Selectable(itr->first.c_str(), is_selected)) {
				current = itr->first;
				changed = true;
			}
			if (is_selected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();

	}
	return changed;
}
#endif // _DEBUG

bool PolygonMeshManager::IsRegisteredUnlocking(const std::string& meshName) {
	return GetInstance().meshInstanceList.contains(meshName);
}
