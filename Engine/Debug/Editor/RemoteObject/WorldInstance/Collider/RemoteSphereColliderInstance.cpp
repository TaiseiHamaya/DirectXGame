#ifdef DEBUG_FEATURES_ENABLE

#include "RemoteSphereColliderInstance.h"

#include "../../../Window/EditorSceneView.h"

void RemoteSphereColliderInstance::update_preview(Reference<RemoteWorldObject> world, Reference<Affine> parentAffine) {
	IRemoteInstance<SphereCollider, void*>::update_preview(world, parentAffine);
	Affine primitiveAffine = Affine::FromScale(Vector3{ radius.cget(), radius.cget(), radius.cget() }) * Affine::FromTranslate(worldAffine.get_origin());
	sceneView->write_primitive(world, "SphereCollider", primitiveAffine);
}

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

#endif // DEBUG_FEATURES_ENABLE
