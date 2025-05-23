#pragma once

#include <filesystem>

#include <json.hpp>

#include <Library/Utility/Tools/ConstructorMacro.h>

#include "../RemoteObject/RemoteSceneObject.h"

class EditorSceneSerializer final {
public:
	EditorSceneSerializer() = default;
	~EditorSceneSerializer() = default;

	__CLASS_NON_COPYABLE(EditorSceneSerializer)

public:
	RemoteSceneObject serialize(std::filesystem::path sceneJson);

	void deserialize(const RemoteSceneObject& scene);

	IRemoteObject generate_object(const nlohmann::json& json);

private:
};
