#include "RemoteSphereColliderInstance.h"

void RemoteSphereColliderInstance::draw_inspector() {
	ImGui::Text("Type : SphereCollider");

	hierarchyName.show_gui();
	ImGui::Separator();

	transform.show_gui();

	ImGui::Separator();
	radius.show_gui();
}

nlohmann::json RemoteSphereColliderInstance::serialize() const {
	nlohmann::json result;

	result.update(transform);
	result.update(hierarchyName);
	result.update(radius);
	result["Type"] = 30;
	result["Children"] = nlohmann::json::array();
	for (const auto& child : children) {
		result["Children"].emplace_back(child->serialize());
	}

	return result;
}

void RemoteSphereColliderInstance::set_editor_world_view(Reference<EditorWorldView> worldView, Reference<const Affine> parentAffine) {
	Affine affine = Affine::FromTransform3D(transform.cget());
	if (parentAffine) {
		affine *= *parentAffine;
	}
	affine = Affine::FromScale(Vector3{ radius.cget(), radius.cget(), radius.cget() }) * affine;

	worldView->register_primitive("SphereCollider", affine);

	for (const auto& child : children) {
		if (child) {
			child->set_editor_world_view(worldView, affine);
		}
	}
}
