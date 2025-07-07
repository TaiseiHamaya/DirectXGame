#include "RemoteCamera3dInstance.h"

void RemoteCamera3dInstance::draw_inspector() {
	ImGui::Text("Type : Camera3D");

	hierarchyName.show_gui();
	ImGui::Separator();

	transform.show_gui();

	ImGui::Separator();
	fovY.show_gui();
	aspectRatio.show_gui();
	nearClip.show_gui();
	farClip.show_gui();
}

nlohmann::json RemoteCamera3dInstance::serialize() const {
	nlohmann::json result;

	result.update(transform);
	result.update(hierarchyName);
	result.update(fovY);
	result.update(aspectRatio);
	result.update(nearClip);
	result.update(farClip);
	result["Type"] = 20;
	result["Children"] = nlohmann::json::array();
	for (const auto& child : children) {
		result["Children"].emplace_back(child->serialize());
	}

	return result;
}
