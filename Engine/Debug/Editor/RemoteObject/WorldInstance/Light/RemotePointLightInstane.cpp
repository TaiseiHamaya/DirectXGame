#ifdef DEBUG_FEATURES_ENABLE

#include "RemotePointLightInstane.h"

#include "../../../Window/EditorSceneView.h"

#define COLOR3_SERIALIZER
#include "Engine/Assets/Json/JsonSerializer.h"

void RemotePointLightInstane::update_preview(Reference<RemoteWorldObject> world, Reference<Affine> parentAffine) {
	IRemoteInstance<PointLightInstance, void*>::update_preview(world, parentAffine);

	Affine primitiveAffine = Affine::FromScale(Vector3{ radius.cget(), radius.cget(), radius.cget() }) * Affine::FromTranslate(worldAffine.get_origin());
	sceneView->write_primitive(world, "Sphere", primitiveAffine);
}

void RemotePointLightInstane::draw_inspector() {
	ImGui::Text("Type : PointLight");

	hierarchyName.show_gui();
	ImGui::Separator();

	transform.show_gui();

	ImGui::Separator();
	color.show_gui();
	intensity.show_gui();
	radius.show_gui();
	decay.show_gui();

	ImGui::Text("Influence Layer");
	for (i32 i = 0; i < sizeof(u32); ++i) {
		if (ImGui::Button(std::format("{}", i).c_str())) {
			influenceLayer = influenceLayer ^ (1u << i);
		}
		if (i % 8 != 7) {
			ImGui::SameLine();
		}
	}
}

nlohmann::json RemotePointLightInstane::serialize() const {
	nlohmann::json result;

	result.update(transform);
	result.update(hierarchyName);
	result.update(color);
	result.update(intensity);
	result.update(radius);
	result.update(decay);
	result.update(influenceLayer);
	result["Type"] = instance_type();
	result["Children"] = nlohmann::json::array();
	for (const auto& child : children) {
		result["Children"].emplace_back(child->serialize());
	}

	return result;
}

#endif // DEBUG_FEATURES_ENABLE
