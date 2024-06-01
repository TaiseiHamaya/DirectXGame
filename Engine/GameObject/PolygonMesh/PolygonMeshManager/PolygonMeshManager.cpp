#include "PolygonMeshManager.h"

#include <format>
#include <mutex>

#include "Engine/GameObject/PolygonMesh/PolygonMesh.h"
#include "Engine/Utility/BackgroundLoader/BackgroundLoader.h"
#include "Engine/Utility/Utility.h"

#ifdef _DEBUG
#include "externals/imgui/imgui.h"
#endif // _DEBUG

std::mutex meshMutex;

PolygonMeshManager::PolygonMeshManager() = default;

PolygonMeshManager::~PolygonMeshManager() = default;

PolygonMeshManager& PolygonMeshManager::GetInstance() {
	static std::unique_ptr<PolygonMeshManager> instance{ new PolygonMeshManager };
	return *instance;
}

void PolygonMeshManager::RegisterLoadQue(const std::string& directoryPath, const std::string& fileName) {
	std::lock_guard<std::mutex> lock{ meshMutex };
	// ロード済みの場合は何もしない
	if (IsRegistered(fileName)) {
		return;
	}
	BackgroundLoader::RegisterLoadQue(LoadEvent::LoadPolygonMesh, directoryPath, fileName);
}

std::weak_ptr<PolygonMesh> PolygonMeshManager::GetPolygonMesh(const std::string& meshName) {
	std::lock_guard<std::mutex> lock{ meshMutex };
	if (IsRegistered(meshName)) {
		return GetInstance().meshInstanceList.at(meshName);
	}
	else {
		// 存在しないメッシュを呼び出したらエラー用メッシュを使用する
		Log(std::format("Unloading polygon mesh. Name-\'{:}\'\n", meshName));
		return GetInstance().meshInstanceList.at("ErrorObject.obj");
	}
}

void PolygonMeshManager::ResetTextureData() {
	for (auto dataItr = GetInstance().meshInstanceList.begin(); dataItr != GetInstance().meshInstanceList.end(); ++dataItr) {
		dataItr->second->reset_data();
	}
}

bool PolygonMeshManager::IsRegistered(const std::string& meshName) {
	return GetInstance().meshRegisteredList.find(meshName) != GetInstance().meshRegisteredList.end();
}

void PolygonMeshManager::Transfer(const std::string& name, std::shared_ptr<PolygonMesh>& data) {
	std::lock_guard<std::mutex> lock{ meshMutex };

	GetInstance().meshInstanceList.emplace(name, data);
	GetInstance().meshRegisteredList.emplace(name);
}

#ifdef _DEBUG
bool PolygonMeshManager::MeshListGui(std::string& current) {
	bool changed = false;

	std::lock_guard<std::mutex> lock{ meshMutex };
	if(ImGui::BeginCombo("MeshList", current.c_str())) {
		auto&& list = GetInstance().meshRegisteredList;
		for (auto itr = list.begin(); itr != list.end(); ++itr) {
			bool is_selected = (current == *itr);
			if (ImGui::Selectable(itr->c_str(), is_selected)) {
				current = *itr;
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
