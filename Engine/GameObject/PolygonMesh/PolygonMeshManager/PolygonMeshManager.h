#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

class PolygonMesh;

class PolygonMeshManager final {
private:
	PolygonMeshManager();

public:
	~PolygonMeshManager();

private:
	PolygonMeshManager(const PolygonMeshManager&) = delete;
	PolygonMeshManager& operator=(const PolygonMeshManager&) = delete;

public:
	static PolygonMeshManager& GetInstance();
	static void RegisterLoadQue(const std::string& directoryPath, const std::string& fileName);
	static std::weak_ptr<PolygonMesh> GetPolygonMesh(const std::string& meshName);
	static void ResetTextureData();
	static bool IsRegistered(const std::string& meshName);
	static void Transfer(const std::string& name, std::shared_ptr<PolygonMesh>& data);

#ifdef _DEBUG
	static bool MeshListGui(std::string& current);
#endif // _DEBUG

private:

private:
	std::unordered_map<std::string, std::shared_ptr<PolygonMesh>> meshInstanceList;
	std::unordered_set<std::string> meshRegisteredList;
};
