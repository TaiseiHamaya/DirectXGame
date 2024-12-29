#include "PolygonMeshManager.h"

#include <mutex>

#include "Engine/Debug/Output.h"
#include "Engine/Resources/BackgroundLoader/BackgroundLoader.h"
#include "Engine/Utility/Tools/SmartPointer.h"
#include "PolygonMesh.h"
#include "PolygonMeshBuilder.h"

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

void PolygonMeshManager::RegisterLoadQue(const std::filesystem::path& filePath) {
	// ロード済みの場合は何もしない
	if (IsRegistered(filePath.filename().string())) {
		return;
	}
	// BackgroundLoaderにイベント送信
	BackgroundLoader::RegisterLoadQue(
		eps::CreateUnique<PolygonMeshBuilder>(filePath)
	);
}

std::shared_ptr<const PolygonMesh> PolygonMeshManager::GetPolygonMesh(const std::string& meshName) {
	std::lock_guard<std::mutex> lock{ meshMutex }; 
	if (IsRegisteredNonlocking(meshName)) {
		return GetInstance().meshInstanceList.at(meshName);
	}
	else {
		// 存在しないメッシュを呼び出したらエラー用メッシュを使用する
		Console("Warning : Unknown polygon mesh. Name-\'{:}\'\n", meshName);
		return GetInstance().meshInstanceList.at("ErrorObject.obj");
	}
}

bool PolygonMeshManager::IsRegistered(const std::string& meshName) {
	std::lock_guard<std::mutex> lock{ meshMutex };
	return IsRegisteredNonlocking(meshName);
}

void PolygonMeshManager::Transfer(const std::string& name, std::shared_ptr<PolygonMesh>& data) {
	std::lock_guard<std::mutex> lock{ meshMutex };
	Console("Transfer new PolygonMesh. Name-\'{:}\', Address-\'{:}\'\n", name, (void*)data.get());
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

bool PolygonMeshManager::IsRegisteredNonlocking(const std::string& meshName) {
	return GetInstance().meshInstanceList.contains(meshName);
}
