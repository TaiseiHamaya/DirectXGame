#include "WorldInstance.h"

#define TRANSFORM3D_SERIALIZER
#include <Engine/Resources/Json/JsonSerializer.h>

WorldInstance::WorldInstance() {
	hierarchy.initialize(affine);
}

void WorldInstance::update_affine() {
	if (!isActive) {
		return;
	}
	affine = create_world_affine();
}

void WorldInstance::look_at(const WorldInstance& rhs, const Vector3& upward) noexcept {
	look_at(rhs.world_position(), upward);
}

// 既知の不具合 : 特定環境でlook_atが正しくならない場合がある
void WorldInstance::look_at(const Vector3& point, const Vector3& upward) noexcept {
	Vector3 localForward;
	Vector3 localUpward;
	if (hierarchy.has_parent()) {
		Affine parentInversedWorldAffine = hierarchy.parent_affine().inverse_fast();
		Vector3 rhsObjectCoordinatePosition = point * parentInversedWorldAffine;
		localUpward = upward * parentInversedWorldAffine.get_basis();
		localForward = (rhsObjectCoordinatePosition - transform.get_translate()).normalize_safe();
	}
	else {
		localUpward = upward;
		localForward = (point - transform.get_translate()).normalize_safe();
	}
	transform.set_quaternion(Quaternion::LookForward(localForward, localUpward));
}

Affine WorldInstance::create_world_affine() const {
	Affine result = Affine::FromTransform3D(transform);
	if (hierarchy.has_parent()) {
		result *= hierarchy.parent_affine();
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
