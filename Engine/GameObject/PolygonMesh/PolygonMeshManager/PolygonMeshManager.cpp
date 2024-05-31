#include "PolygonMeshManager.h"

#include <cassert>
#include <mutex>

#include "Engine/GameObject/PolygonMesh/PolygonMesh.h"
#include "Engine/Utility/BackgroundLoader/BackgroundLoader.h"

std::mutex meshMutex;

PolygonMeshManager::PolygonMeshManager() = default;

PolygonMeshManager::~PolygonMeshManager() = default;

PolygonMeshManager& PolygonMeshManager::GetInstance() {
	static std::unique_ptr<PolygonMeshManager> instance{ new PolygonMeshManager };
	return *instance;
}

void PolygonMeshManager::LoadPolygonMesh(const std::string& directoryPath, const std::string& fileName) {
	// ロード済みの場合は何もしない
	if (GetInstance().meshRegisteredList.find(fileName) != GetInstance().meshRegisteredList.end()) {
		return;
	}

	BackgroundLoader::RegisterLoadQue(LoadEvent::LoadPolygonMesh, directoryPath, fileName);
}

std::weak_ptr<PolygonMesh> PolygonMeshManager::GetPolygonMesh(const std::string& meshName) {
	assert(GetInstance().meshRegisteredList.find(meshName) != GetInstance().meshRegisteredList.end());
	return GetInstance().meshInstanceList.at(meshName);
}

void PolygonMeshManager::Transfer(const std::string& fileName, std::shared_ptr<PolygonMesh> meshData) {
	std::lock_guard<std::mutex> lock{ meshMutex };

	GetInstance().meshInstanceList.emplace(fileName, meshData);
	GetInstance().meshRegisteredList.emplace(fileName);
}
