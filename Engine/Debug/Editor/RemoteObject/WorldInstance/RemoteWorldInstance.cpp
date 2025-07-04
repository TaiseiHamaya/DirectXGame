#ifdef DEBUG_FEATURES_ENABLE

#include "RemoteWorldInstance.h"

#define TRANSFORM3D_SERIALIZER
#include "Engine/Assets/Json/JsonSerializer.h"

RemoteWorldInstance::RemoteWorldInstance() = default;
RemoteWorldInstance::~RemoteWorldInstance() = default;

void RemoteWorldInstance::draw_inspector() {
	ImGui::Text("Type : WorldInstance");

	hierarchyName.show_gui();
	ImGui::Separator();

	transform.show_gui();
}

nlohmann::json RemoteWorldInstance::serialize() const {
	nlohmann::json result;

	result.update(transform);
	result.update(hierarchyName);
	result["Type"] = 0;
	result["Children"] = nlohmann::json::array();
	for (const auto& child : children) {
		result["Children"].emplace_back(child->serialize());
	}

	return result;
}

#endif // DEBUG_FEATURES_ENABLE
