#include "WorldInstanceLoader.h"

#include "Engine/Assets/Texture/TextureLibrary.h"
#include "Engine/Module/Manager/World/WorldRoot.h"
#include "Engine/Module/World/Camera/Camera3D.h"
#include "Engine/Module/World/Mesh/SkinningMeshInstance.h"
#include "Engine/Module/World/Mesh/StaticMeshInstance.h"
#include "Engine/Module/World/Collision/Collider/AABBCollider.h"
#include "Engine/Module/World/Collision/Collider/SphereCollider.h"
#include "Engine/Module/World/WorldInstance/WorldInstance.h"

#define TRANSFORM3D_SERIALIZER
#define TRANSFORM2D_SERIALIZER
#define COLOR3_SERIALIZER
#include "Engine/Assets/Json/JsonSerializer.h"

void WorldInstanceLoader::setup(Reference<WorldRoot> worldRoot_) {
	worldRoot = worldRoot_;
}

void WorldInstanceLoader::entry_point(const nlohmann::json& json, Reference<WorldInstance> parent) {
	if (!json.contains("Type")) {
		return;
	}
	u64 type = json.at("Type").get<u64>();
	switch (type) {
	case 0:
	{
		create_world_instance(json, parent);
		break;
	}
	case 10: // Mesh
	{
		create_static_mesh_instance(json, parent);
		break;
	}
	case 11: // SkinMesh
	{
		create_skinning_mesh_instance(json, parent);
		break;
	}
	case 12: // Rect3D
	{
		break;
	}
	case 13: // Skybox
	{
		break;
	}
	case 20: // Camera3D
	{
		create_camera3d_instance(json, parent);
		break;
	}
	case 21: // Camera2D
	{
		break;
	}
	case 30: // SphereCollider
	{
		create_sphere_collider_instance(json, parent);
		break;
	}
	case 31: // AABB Collider
	{
		create_aabb_collider_instance(json, parent);
		break;
	}
	case 40: // DirectionalLight
	{
		break;
	}
	case 90:
	{
		return calclate_folder_hierarchy(json, parent);
	}
	default:
	{
		break;
	}
	}
}

void WorldInstanceLoader::create_world_instance(const nlohmann::json& json, Reference<WorldInstance> parent) {
	auto instance = worldRoot->instantiate<WorldInstance>(parent);

	instance->get_transform().copy(json["Local transform"].get<Transform3D>());

	if (json.contains("Children") && json["Children"].is_array()) {
		for (const nlohmann::json& instanceJson : json["Children"]) {
			entry_point(instanceJson, instance);
		}
	}
}

void WorldInstanceLoader::create_static_mesh_instance(const nlohmann::json& json, Reference<WorldInstance> parent) {
	auto instance = worldRoot->instantiate<StaticMeshInstance>(parent);

	instance->get_transform().copy(json["Local transform"].get<Transform3D>());
	instance->reset_mesh(json.value("MeshName", ""));
	instance->set_draw(json.value("IsDraw", true));
	instance->set_layer(json.value("Layer", 0u));

	if (json.contains("Materials") && json["Materials"].is_array()) {
		const auto& materialsJson = json["Materials"];
		for (u32 i = 0; i < materialsJson.size() && i < instance->get_materials().size(); ++i) {
			auto& material = instance->get_materials()[i];
			const auto& materialJson = materialsJson[i];
			if (materialJson.contains("Texture")) {
				material.texture = TextureLibrary::GetTexture(materialJson["Texture"].get<std::string>());
			}
			if (materialJson.contains("Color")) {
				materialJson["Color"].get_to(material.color);
			}
			if (materialJson.contains("UV Transform")) {
				materialJson["UV Transform"].get_to(material.uvTransform);
			}
			if (materialJson.contains("LightingType")) {
				material.lightingType = static_cast<LighingType>(materialJson["LightingType"].get<std::underlying_type_t<LighingType>>());
			}
			if (materialJson.contains("Shininess")) {
				materialJson["Shininess"].get_to(material.shininess);
			}
		}
	}

	if (json.contains("Children") && json["Children"].is_array()) {
		for (const nlohmann::json& instanceJson : json["Children"]) {
			entry_point(instanceJson, instance);
		}
	}
}

void WorldInstanceLoader::create_skinning_mesh_instance(const nlohmann::json& json, Reference<WorldInstance> parent) {
	auto instance = worldRoot->instantiate<SkinningMeshInstance>(parent);

	instance->get_transform().copy(json["Local transform"].get<Transform3D>());
	instance->reset_animated_mesh(json.value("MeshName", ""), json.value("AnimationName", ""), json.value("IsLoop", false));
	instance->set_draw(json.value("IsDraw", true));
	instance->set_layer(json.value("Layer", 0u));

	if (json.contains("Materials") && json["Materials"].is_array()) {
		const auto& materialsJson = json["Materials"];
		for (u32 i = 0; i < materialsJson.size() && i < instance->get_materials().size(); ++i) {
			auto& material = instance->get_materials()[i];
			const auto& materialJson = materialsJson[i];
			if (materialJson.contains("Texture")) {
				material.texture = TextureLibrary::GetTexture(materialJson["Texture"].get<std::string>());
			}
			if (materialJson.contains("Color")) {
				materialJson["Color"].get_to(material.color);
			}
			if (materialJson.contains("UV Transform")) {
				materialJson["UV Transform"].get_to(material.uvTransform);
			}
			if (materialJson.contains("LightingType")) {
				material.lightingType = static_cast<LighingType>(materialJson["LightingType"].get<std::underlying_type_t<LighingType>>());
			}
			if (materialJson.contains("Shininess")) {
				materialJson["Shininess"].get_to(material.shininess);
			}
		}
	}

	if (json.contains("Children") && json["Children"].is_array()) {
		for (const nlohmann::json& instanceJson : json["Children"]) {
			entry_point(instanceJson, instance);
		}
	}
}

void WorldInstanceLoader::create_camera3d_instance(const nlohmann::json& json, Reference<WorldInstance> parent) {
	auto instance = worldRoot->instantiate<Camera3D>(parent);

	instance->get_transform().copy(json["Local transform"].get<Transform3D>());
	instance->set_perspective_fov_info(
		json.value("FovY", 0.45f),
		json.value("AspectRatio", 1.7777f),
		json.value("NearClip", 0.1f),
		json.value("FarClip", 1000.0f)
	);

	if (json.contains("Children") && json["Children"].is_array()) {
		for (const nlohmann::json& instanceJson : json["Children"]) {
			entry_point(instanceJson, instance);
		}
	}
}

void WorldInstanceLoader::create_sphere_collider_instance(const nlohmann::json& json, Reference<WorldInstance> parent) {
	auto instance = worldRoot->instantiate<SphereCollider>(parent);

	instance->get_transform().copy(json["Local transform"].get<Transform3D>());
	instance->set_radius(json.value("Radius", 1.0f));

	if (json.contains("Children") && json["Children"].is_array()) {
		for (const nlohmann::json& instanceJson : json["Children"]) {
			entry_point(instanceJson, instance);
		}
	}
}

void WorldInstanceLoader::create_aabb_collider_instance(const nlohmann::json& json, Reference<WorldInstance> parent) {
	auto instance = worldRoot->instantiate<AABBCollider>(parent, json.value("Size", CVector3::ONE), json.value("Offset", CVector3::ZERO));
	
	instance->get_transform().copy(json["Local transform"].get<Transform3D>());

	if (json.contains("Children") && json["Children"].is_array()) {
		for (const nlohmann::json& instanceJson : json["Children"]) {
			entry_point(instanceJson, instance);
		}
	}
}

void WorldInstanceLoader::calclate_folder_hierarchy(const nlohmann::json& json, Reference<WorldInstance> parent) {
	if (json.contains("Children") && json["Children"].is_array()) {
		for (const nlohmann::json& instanceJson : json["Children"]) {
			entry_point(instanceJson, parent);
		}
	}
}
