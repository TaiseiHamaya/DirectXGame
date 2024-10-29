#include "PolygonMeshManager.h"

#include <mutex>

#include "Engine/Resources/PolygonMesh/PolygonMesh.h"
#include "Engine/Resources/BackgroundLoader/BackgroundLoader.h"
#include "Engine/Debug/Output.h"

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

std::mutex meshMutex;

PolygonMeshManager::PolygonMeshManager() noexcept = default;

PolygonMeshManager::~PolygonMeshManager() noexcept = default;

PolygonMeshManager& PolygonMeshManager::GetInstance() noexcept {
	static PolygonMeshManager instance{};
	return instance;
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
	if (IsRegisteredNolocking(meshName)) {
		return GetInstance().meshInstanceList.at(meshName);
	}
	else {
		// 存在しないメッシュを呼び出したらエラー用メッシュを使用する
		Console("[PolygonMeshManager] Unloading polygon mesh. Name-\'{:}\'\n", meshName);
		return GetInstance().meshInstanceList.at("ErrorObject.obj");
	}
}

bool PolygonMeshManager::IsRegistered(const std::string& meshName) {
	std::lock_guard<std::mutex> lock{ meshMutex };
	return IsRegisteredNolocking(meshName);
}

void PolygonMeshManager::Transfer(const std::string& name, std::shared_ptr<PolygonMesh>& data) {
	std::lock_guard<std::mutex> lock{ meshMutex };
	Console("[PolygonMeshManager] Transfer new PolygonMesh. Name-\'{:}\', Address-\'{:}\'\n", name, (void*)data.get());
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

bool PolygonMeshManager::IsRegisteredNolocking(const std::string& meshName) {
	return GetInstance().meshInstanceList.contains(meshName);
}
