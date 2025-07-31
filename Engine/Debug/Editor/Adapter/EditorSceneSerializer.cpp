#ifdef DEBUG_FEATURES_ENABLE

#include "EditorSceneSerializer.h"

#include "../RemoteObject/FolderObject.h"
#include "../RemoteObject/RemoteErrorObject.h"
#include "../RemoteObject/RemoteSceneObject.h"
#include "../RemoteObject/RemoteWorldObject.h"
#include "../RemoteObject/WorldInstance/Camera/RemoteCamera3dInstance.h"
#include "../RemoteObject/WorldInstance/Mesh/RemoteSkinningMeshInstance.h"
#include "../RemoteObject/WorldInstance/Mesh/RemoteStaticMeshInstance.h"
#include "../RemoteObject/WorldInstance/Collider/RemoteSphereColliderInstance.h"
#include "../RemoteObject/WorldInstance/Collider/RemoteAABBColliderInstance.h"
#include "../RemoteObject/WorldInstance/RemoteWorldInstance.h"

#include "Engine/Application/Output.h"
#include "Engine/Assets/Animation/Skeleton/SkeletonLibrary.h"

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

	json.get_to(world->numLayer);
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
		return CreateRemoteStaticMeshInstance(json);
	}
	case 11: // SkinMesh
	{
		return CreateRemoteSkinningMeshInstance(json);
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
		return CreateRemoteCamera3DInstance(json);
	}
	case 21: // Camera2D
	{
		return std::make_unique<RemoteErrorObject>("Camera2D is not defined.");
	}
	case 30: // SphereCollider
	{
		return CreateRemoteSphereColliderInstance(json);
	}
	case 31: // AABB Collider
	{
		return CreateRemoteAABBColliderInstance(json);
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

std::unique_ptr<IRemoteObject> EditorSceneSerializer::CreateRemoteStaticMeshInstance(const nlohmann::json& json) {
	std::unique_ptr<RemoteStaticMeshInstance> result = std::make_unique<RemoteStaticMeshInstance>();
	json.get_to(result->hierarchyName);
	if (json.contains("Children") && json["Children"].is_array()) {
		for (const nlohmann::json& instance : json["Children"]) {
			result->add_child(CreateRemoteObject(instance));
		}
	}
	json.get_to(result->transform);
	json.get_to(result->isDraw);
	if (json.contains("MeshName")) {
		json["MeshName"].get_to(result->meshName);
	}
	json.get_to(result->layer);
	if (json.contains("Materials") && json["Materials"].is_array()) {
		for (const nlohmann::json& jMaterial : json["Materials"]) {
			RemoteStaticMeshInstance::Material& material = result->materials.emplace_back();
			if (jMaterial.contains("Texture")) {
				material.texture = jMaterial.at("Texture").get<std::string>();
			}
			if (jMaterial.contains("Color")) {
				jMaterial.at("Color").get_to(material.color);
			}
			if (jMaterial.contains("UV Transform")) {
				jMaterial.at("UV Transform").get_to(material.uvTransform);
			}
			if (jMaterial.contains("LightingType")) {
				material.lightingType = static_cast<LighingType>(jMaterial.at("LightingType").get<std::underlying_type_t<LighingType>>());
			}
			if (jMaterial.contains("Shininess")) {
				jMaterial.at("Shininess").get_to(material.shininess);
			}
		}
	}

	return result;
}

std::unique_ptr<IRemoteObject> EditorSceneSerializer::CreateRemoteSkinningMeshInstance(const nlohmann::json& json) {
	std::unique_ptr<RemoteSkinningMeshInstance> result = std::make_unique<RemoteSkinningMeshInstance>();
	json.get_to(result->hierarchyName);
	if (json.contains("Children") && json["Children"].is_array()) {
		for (const nlohmann::json& instance : json["Children"]) {
			result->add_child(CreateRemoteObject(instance));
		}
	}
	json.get_to(result->transform);
	json.get_to(result->isDraw);
	if (json.contains("MeshName")) {
		json["MeshName"].get_to(result->meshName);
	}
	result->skeleton = SkeletonLibrary::GetSkeleton(result->meshName);
	json.get_to(result->layer);
	if (json.contains("Materials") && json["Materials"].is_array()) {
		for (const nlohmann::json& jMaterial : json["Materials"]) {
			RemoteSkinningMeshInstance::Material& material = result->materials.emplace_back();
			if (jMaterial.contains("Texture")) {
				material.texture = jMaterial.at("Texture").get<std::string>();
			}
			if (jMaterial.contains("Color")) {
				jMaterial.at("Color").get_to(material.color);
			}
			if (jMaterial.contains("UV Transform")) {
				jMaterial.at("UV Transform").get_to(material.uvTransform);
			}
			if (jMaterial.contains("LightingType")) {
				material.lightingType = static_cast<LighingType>(jMaterial.at("LightingType").get<std::underlying_type_t<LighingType>>());
			}
			if (jMaterial.contains("Shininess")) {
				jMaterial.at("Shininess").get_to(material.shininess);
			}
		}
	}
	if (json.contains("AnimationName")) {
		json["AnimationName"].get_to(result->animationName);
	}
	json.get_to(result->isLoop);

	return result;
}

std::unique_ptr<IRemoteObject> EditorSceneSerializer::CreateRemoteCamera3DInstance(const nlohmann::json& json) {
	std::unique_ptr<RemoteCamera3dInstance> result = std::make_unique<RemoteCamera3dInstance>();
	json.get_to(result->hierarchyName);
	if (json.contains("Children") && json["Children"].is_array()) {
		for (const nlohmann::json& instance : json["Children"]) {
			result->add_child(CreateRemoteObject(instance));
		}
	}
	json.get_to(result->transform);
	json.get_to(result->fovY);
	json.get_to(result->aspectRatio);
	json.get_to(result->nearClip);
	json.get_to(result->farClip);

	return result;
}

std::unique_ptr<IRemoteObject> EditorSceneSerializer::CreateRemoteSphereColliderInstance(const nlohmann::json& json) {
	std::unique_ptr<RemoteSphereColliderInstance> result = std::make_unique<RemoteSphereColliderInstance>();
	json.get_to(result->hierarchyName);
	if (json.contains("Children") && json["Children"].is_array()) {
		for (const nlohmann::json& instance : json["Children"]) {
			result->add_child(CreateRemoteObject(instance));
		}
	}
	json.get_to(result->transform);
	json.get_to(result->radius);

	return result;
}

std::unique_ptr<IRemoteObject> EditorSceneSerializer::CreateRemoteAABBColliderInstance(const nlohmann::json& json) {
	std::unique_ptr<RemoteAABBColliderInstance> result = std::make_unique<RemoteAABBColliderInstance>();
	json.get_to(result->hierarchyName);
	if (json.contains("Children") && json["Children"].is_array()) {
		for (const nlohmann::json& instance : json["Children"]) {
			result->add_child(CreateRemoteObject(instance));
		}
	}
	json.get_to(result->transform);
	json.get_to(result->size);
	json.get_to(result->offset);

	return result;
}

#endif // DEBUG_FEATURES_ENABLE
