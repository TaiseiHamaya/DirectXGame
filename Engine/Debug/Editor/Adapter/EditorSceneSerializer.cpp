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
	if (json.contains("Name")) {
		scene->hierarchyName = json["Name"];
	}
	else {
		scene->hierarchyName = "Scene (Name is not found.)";
	}
	if (json.contains("Worlds") && json["Worlds"].is_array()) {
		for (const nlohmann::json& world : json["Worlds"]) {
			scene->add_child(EditorSceneSerializer::CreateRemoteWorld(world));
		}
	}
	return scene;
}

std::unique_ptr<RemoteWorldObject> EditorSceneSerializer::CreateRemoteWorld(const nlohmann::json& json) {
	std::unique_ptr<RemoteWorldObject> world = std::make_unique<RemoteWorldObject>();
	if (json.contains("Name") && json["Name"].is_string()) {
		world->hierarchyName = json["Name"];
	}
	else {
		world->hierarchyName = "World";
	}
	if (json.contains("Instances") && json["Instances"].is_array()) {
		for (const nlohmann::json& instance : json["Instances"]) {
			world->add_child(CreateRemoteObject(instance));
		}
	}
	return world;
}

std::unique_ptr<IRemoteObject> EditorSceneSerializer::CreateRemoteObject(const nlohmann::json& json) {
	if (!json.contains("Type")) {
		Warning("Remote object type is not found.");
		return nullptr;
	}
	u64 type = json.at("Type").get<u64>();
	switch (type) {
	case 0:
	{
		return CreateRemoteInstance(json);
	}
	case 10: // Mesh
	{
		return std::make_unique<RemoteErrorObject>();
	}
	case 11: // SkinMesh
	{
		return std::make_unique<RemoteErrorObject>();
	}
	case 12: // Rect3D
	{
		return std::make_unique<RemoteErrorObject>();
	}
	case 13: // Skybox
	{
		return std::make_unique<RemoteErrorObject>();
	}
	case 20: // Camera3D
	{
		return std::make_unique<RemoteErrorObject>();
	}
	case 21: // Camera2D
	{
		return std::make_unique<RemoteErrorObject>();
	}
	case 30: // SphereCollider
	{
		return std::make_unique<RemoteErrorObject>();
	}
	case 31: // AABB Collider
	{
		return std::make_unique<RemoteErrorObject>();
	}
	case 40: // DirectionalLight
	{
		return std::make_unique<RemoteErrorObject>();
	}
	case 90:
	{
		return CreateRemoteFolder(json);
	}
	default:
	{
		Warning("Unknown remote object type.");
		return std::make_unique<RemoteErrorObject>();
	}
	}
}

void EditorSceneSerializer::SaveToJson(Reference<const RemoteSceneObject> scene) {
	nlohmann::json root;
	root = SaveRemoteScene(scene);
}

std::unique_ptr<IRemoteObject> EditorSceneSerializer::CreateRemoteFolder(const nlohmann::json& json) {
	std::unique_ptr<FolderObject> folder = std::make_unique<FolderObject>();
	folder->hierarchyName = json["Name"];
	if (json.contains("Children") && json["Children"].is_array()) {
		for (const nlohmann::json& instance : json["Children"]) {
			folder->add_child(CreateRemoteObject(instance));
		}
	}
	return folder;
}

std::unique_ptr<IRemoteObject> EditorSceneSerializer::CreateRemoteInstance(const nlohmann::json& json) {
	std::unique_ptr<RemoteWorldInstance> result = std::make_unique<RemoteWorldInstance>();
	result->hierarchyName = json["Name"];
	if (json.contains("Children") && json["Children"].is_array()) {
		for (const nlohmann::json& instance : json["Children"]) {
			result->add_child(CreateRemoteObject(instance));
		}
	}
	if (json.contains("Transform")) {
		result->transform = json["Transform"];
	}

	return result;
}

nlohmann::json EditorSceneSerializer::SaveRemoteScene(Reference<const RemoteSceneObject> scene) {
	nlohmann::json result;
	result["Name"] = scene->hierarchyName;
	result["Worlds"] = nlohmann::json::array();
	for (const auto& world : scene->remoteWorlds) {
		result["Worlds"].emplace_back(SaveRemoteWorld(world));
	}
	return result;
}

nlohmann::json EditorSceneSerializer::SaveRemoteWorld(Reference<const RemoteWorldObject> world) {
	nlohmann::json result;
	result["Name"] = world->hierarchyName;
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
