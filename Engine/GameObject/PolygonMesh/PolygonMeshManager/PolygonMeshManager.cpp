#include "PolygonMeshManager.h"

#include <cassert>

#include "Engine/GameObject/PolygonMesh/PolygonMesh.h"

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

	std::shared_ptr<PolygonMesh> mesh{new PolygonMesh};
	mesh->load(directoryPath, fileName);

	GetInstance().meshInstanceList.emplace(fileName, mesh);
	GetInstance().meshRegisteredList.emplace(fileName);
}

std::weak_ptr<PolygonMesh> PolygonMeshManager::GetPolygonMesh(const std::string& meshName) {
	assert(GetInstance().meshRegisteredList.find(meshName) != GetInstance().meshRegisteredList.end());
	return GetInstance().meshInstanceList.at(meshName);
}
