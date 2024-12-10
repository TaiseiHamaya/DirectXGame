#include "WorldInstance.h"

#define TRANSFORM3D_SERIALIZER
#include <Engine/Resources/Json/JsonSerializer.h>

WorldInstance::WorldInstance() {
	hierarchy.initialize(worldMatrix);
}

void WorldInstance::update_matrix() {
	if (!isActive) {
		return;
	}
	worldMatrix = create_world_matrix();
}

void WorldInstance::look_at(const WorldInstance& rhs, const Vector3& upward) noexcept {
	look_at(rhs.world_position(), upward);
}

// 既知の不具合 : 特定環境でlook_atが正しくならない場合がある
void WorldInstance::look_at(const Vector3& point, const Vector3& upward) noexcept {
	Vector3 localForward;
	Vector3 localUpward;
	if (hierarchy.has_parent()) {
		Matrix4x4 parentInversedWorldMatrix = hierarchy.parent_matrix().inverse();
		Vector3 rhsObjectCoordinatePosition = Transform3D::Homogeneous(point, parentInversedWorldMatrix);
		localUpward = Transform3D::HomogeneousVector(upward, parentInversedWorldMatrix);
		localForward = (rhsObjectCoordinatePosition - transform.get_translate()).normalize_safe();
	}
	else {
		localUpward = upward;
		localForward = (point - transform.get_translate()).normalize_safe();
	}
	transform.set_quaternion(Quaternion::LookForward(localForward, localUpward));
}

Matrix4x4 WorldInstance::create_world_matrix() const {
	Matrix4x4 result = transform.create_matrix();
	if (hierarchy.has_parent()) {
		result *= hierarchy.parent_matrix();
	}
	return result;
}

void WorldInstance::from_json(const JsonResource& json_) {
	const nlohmann::json& json = json_.cget();
	if (json.contains("WorldInstance")) {
		transform = json.at("WorldInstance").get<Transform3D>();
	}
}

void WorldInstance::to_json(JsonResource& json) {
	json.get()["WorldInstance"] = transform;
}

#ifdef _DEBUG
#include <imgui.h>

#include <string>
#include <source_location>

using namespace std::literals::string_literals;

void WorldInstance::debug_gui() {
	ImGui::Checkbox("Active", &isActive);
	transform.debug_gui();

	//if (ImGui::Button("SaveJson")) {
	//	constexpr const char* fileName = std::source_location::current().file_name();
	//	JsonResource output{ "WorldInstance"s + fileName };
	//	to_json(output);
	//	output.save();
	//}
}
#endif // _DEBUG
