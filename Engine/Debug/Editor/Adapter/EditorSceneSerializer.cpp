#ifdef DEBUG_FEATURES_ENABLE

#include "EditorSceneSerializer.h"

#include "../RemoteObject/FolderObject.h"
#include "../RemoteObject/RemoteErrorObject.h"
#include "../RemoteObject/RemoteSceneObject.h"
#include "../RemoteObject/RemoteWorldObject.h"
#include "../RemoteObject/WorldInstance/RemoteWorldInstance.h"

#include "Engine/Application/Output.h"

#include <Library/Utility/Template/string_hashed.h>

#define TRANSFORM3D_SERIALIZER
#include "Engine/Assets/Json/JsonSerializer.h"

std::unique_ptr<RemoteSceneObject> EditorSceneSerializer::CreateRemoteScene(const nlohmann::json& json) {
	std::unique_ptr<RemoteSceneObject> scene = std::make_unique<RemoteSceneObject>();
	json.get_to(scene->hierarchyName);
	if (json.contains("Worlds") && json["Worlds"].is_array()) {
		for (const nlohmann::json& world : json["Worlds"]) {
			scene->add_child(EditorSceneSerializer::CreateRemoteWorld(world));
		}
	}
	return scene;
}

std::unique_ptr<RemoteWorldObject> EditorSceneSerializer::CreateRemoteWorld(const nlohmann::json& json) {
	std::unique_ptr<RemoteWorldObject> world = std::make_unique<RemoteWorldObject>();
	json.get_to(world->hierarchyName);
	if (json.contains("Instances") && json["Instances"].is_array()) {
		for (const nlohmann::json& instance : json["Instances"]) {
			world->add_child(CreateRemoteObject(instance));
		}
	}
	return world;
}

std::unique_ptr<IRemoteObject> EditorSceneSerializer::CreateRemoteObject(const nlohmann::json& json) {
	if (!json.contains("Type")) {
		return std::make_unique<RemoteErrorObject>("Remote object type is not found.");
	}
	u64 type = json.at("Type").get<u64>();
	switch (type) {
	case 0:
	{
		return CreateRemoteInstance(json);
	}
	case 10: // Mesh
	{
		return std::make_unique<RemoteErrorObject>("RemoteStaticMeshInstance is not defined.");
	}
	case 11: // SkinMesh
	{
		return std::make_unique<RemoteErrorObject>("RemoteSkinMesh is not defined.");
	}
	case 12: // Rect3D
	{
		return std::make_unique<RemoteErrorObject>("RemoteRect3D is not defined.");
	}
	case 13: // Skybox
	{
		return std::make_unique<RemoteErrorObject>("RemoteSkybox is not defined.");
	}
	case 20: // Camera3D
	{
		return std::make_unique<RemoteErrorObject>("RemoteCamera3D is not defined.");
	}
	case 21: // Camera2D
	{
		return std::make_unique<RemoteErrorObject>("RemoteSphereCollider is not defined.");
	}
	case 30: // SphereCollider
	{
		return std::make_unique<RemoteErrorObject>("RemoteStaticMeshInstance is not defined.");
	}
	case 31: // AABB Collider
	{
		return std::make_unique<RemoteErrorObject>("RemoteAABBCollider is not defined.");
	}
	case 40: // DirectionalLight
	{
		return std::make_unique<RemoteErrorObject>("RemoteDirectionalLight is not defined.");
	}
	case 90:
	{
		return CreateRemoteFolder(json);
	}
	default:
	{
		return std::make_unique<RemoteErrorObject>(std::format("Undefined remote object type.({})", type));
	}
	}
}

void EditorSceneSerializer::SaveToJson(Reference<const RemoteSceneObject> scene) {
	nlohmann::json root;
	root = SaveRemoteScene(scene);
}

std::unique_ptr<IRemoteObject> EditorSceneSerializer::CreateRemoteFolder(const nlohmann::json& json) {
	std::unique_ptr<FolderObject> folder = std::make_unique<FolderObject>();
	json.get_to(folder->hierarchyName);
	if (json.contains("Children") && json["Children"].is_array()) {
		for (const nlohmann::json& instance : json["Children"]) {
			folder->add_child(CreateRemoteObject(instance));
		}
	}
	return folder;
}

std::unique_ptr<IRemoteObject> EditorSceneSerializer::CreateRemoteInstance(const nlohmann::json& json) {
	std::unique_ptr<RemoteWorldInstance> result = std::make_unique<RemoteWorldInstance>();
	json.get_to(result->hierarchyName);
	if (json.contains("Children") && json["Children"].is_array()) {
		for (const nlohmann::json& instance : json["Children"]) {
			result->add_child(CreateRemoteObject(instance));
		}
	}
	json.get_to(result->transform);

	return result;
}

nlohmann::json EditorSceneSerializer::SaveRemoteScene(Reference<const RemoteSceneObject> scene) {
	nlohmann::json result;
	result = scene->hierarchyName;
	result["Worlds"] = nlohmann::json::array();
	for (const auto& world : scene->remoteWorlds) {
		result["Worlds"].emplace_back(SaveRemoteWorld(world));
	}
	return result;
}

nlohmann::json EditorSceneSerializer::SaveRemoteWorld(Reference<const RemoteWorldObject> world) {
	nlohmann::json result;
	result = world->hierarchyName;
	result["Instances"] = nlohmann::json::array();
	for (const auto& instance : world->children) {
		//result["Instances"].emplace_back((instance));
	}
	return result;
}

nlohmann::json EditorSceneSerializer::SaveRemoteFolder(Reference<const FolderObject> folder) {
	nlohmann::json result;
	return result;
}

nlohmann::json EditorSceneSerializer::SaveRemoteInstance(Reference<const RemoteWorldInstance> instance) {
	nlohmann::json result;
	return result;
}

#endif // DEBUG_FEATURES_ENABLE
