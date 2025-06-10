#pragma once

#include <memory>

#include <json.hpp>

#include <Library/Utility/Tools/ConstructorMacro.h>

#include "../RemoteObject/RemoteSceneObject.h"
#include "../RemoteObject/RemoteWorldObject.h"

class EditorSceneSerializer final {
public:
	EditorSceneSerializer() = default;
	~EditorSceneSerializer() = default;

	__CLASS_NON_COPYABLE(EditorSceneSerializer)

public:
	static std::unique_ptr<RemoteSceneObject> CreateRemoteScene(const nlohmann::json& json);
	static std::unique_ptr<RemoteWorldObject> CreateRemoteWorld(const nlohmann::json& json);
	static std::unique_ptr<IRemoteObject> CreateRemoteObject(const nlohmann::json& json);

private:
	static std::unique_ptr<IRemoteObject> CreateRemoteFolder(const nlohmann::json& json);
	static std::unique_ptr<IRemoteObject> CreateRemoteInstance(const nlohmann::json& json);
};
