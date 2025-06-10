#include "EditorSceneSerializer.h"

#include "../RemoteObject/RemoteSceneObject.h"
#include "../RemoteObject/RemoteWorldInstance.h"
#include "../RemoteObject/RemoteWorldObject.h"
#include "../RemoteObject/FolderObject.h"

#include <Library/Utility/Template/string_hashed.h>

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
			scene->remoteWorlds.emplace_back(EditorSceneSerializer::CreateRemoteWorld(world));
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
			world->children.emplace_back(CreateRemoteObject(instance));
		}
	}
	return world;
}

std::unique_ptr<IRemoteObject> EditorSceneSerializer::CreateRemoteObject(const nlohmann::json& json) {
	if (!json.contains("Type")) {
		Warning("Remote object type is not found.");
		return nullptr;
	}
	u64 type = eps::string_hash(std::to_string(json.at("Type").get<i32>()));
	switch (type) {
	case eps::string_hash("10"):
	{
		return CreateRemoteInstance(json);
	}
	case eps::string_hash("20"):
	{
		return nullptr;
	}
	case eps::string_hash("30"):
	{
		return nullptr;
	}
	case eps::string_hash("40"):
	{
		return nullptr;
	}
	case eps::string_hash("50"):
	{
		return nullptr;
	}
	case eps::string_hash("90"):
	{
		return CreateRemoteFolder(json);
	}
	default:
	{
		Warning("Unknown remote object type.");
		return nullptr;
	}
	}
}

std::unique_ptr<IRemoteObject> EditorSceneSerializer::CreateRemoteFolder(const nlohmann::json& json) {
	std::unique_ptr<FolderObject> folder = std::make_unique<FolderObject>();
	folder->hierarchyName = json["Name"];
	if (json.contains("Children") && json["Children"].is_array()) {
		for (const nlohmann::json& instance : json["Children"]) {
			folder->children.emplace_back(CreateRemoteObject(instance));
		}
	}
	return folder;
}

std::unique_ptr<IRemoteObject> EditorSceneSerializer::CreateRemoteInstance(const nlohmann::json& json) {
	std::unique_ptr<RemoteWorldInstance> result = std::make_unique<RemoteWorldInstance>();
	result->hierarchyName = json["Name"];
	if (json.contains("Children") && json["Children"].is_array()) {
		for (const nlohmann::json& instance : json["Children"]) {
			result->children.emplace_back(CreateRemoteObject(instance));
		}
	}
	result->
	return result;
}
