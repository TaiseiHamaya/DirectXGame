#include "RemoteAABBColliderInstance.h"

void RemoteAABBColliderInstance::draw_inspector() {
	ImGui::Text("Type : AABBCollider");

	hierarchyName.show_gui();
	ImGui::Separator();

	transform.show_gui();

	ImGui::Separator();
	size.show_gui();
	offset.show_gui();
}

nlohmann::json RemoteAABBColliderInstance::serialize() const {
	nlohmann::json result;

	result.update(transform);
	result.update(hierarchyName);
	result.update(size);
	result.update(offset);
	result["Type"] = 31;
	result["Children"] = nlohmann::json::array();
	for (const auto& child : children) {
		result["Children"].emplace_back(child->serialize());
	}

	return result;
}

void RemoteAABBColliderInstance::set_editor_world_view(Reference<EditorWorldView> worldView, Reference<const Affine> parentAffine) {
	Affine affine = Affine::FromTransform3D(transform.cget());
	if (parentAffine) {
		affine *= *parentAffine;
	}
	affine = Affine::FromSRT(size.cget(), CQuaternion::IDENTITY, Vector3::Multiply(-offset.cget(), size.cget())) * affine;

	worldView->register_primitive("AABBCollider", affine);

	for (const auto& child : children) {
		if (child) {
			child->set_editor_world_view(worldView, affine);
		}
	}
}
