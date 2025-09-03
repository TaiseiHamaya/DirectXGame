#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include <memory>
#include <string>

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
	static std::unique_ptr<RemoteSceneObject> CreateRemoteScene(const std::string& sceneName);
	static std::unique_ptr<RemoteWorldObject> CreateRemoteWorld(const nlohmann::json& json);
	static std::unique_ptr<IRemoteObject> CreateRemoteObject(const nlohmann::json& json);

	//static void SaveToJson(Reference<const RemoteSceneObject> scene);

private:
	static std::unique_ptr<IRemoteObject> CreateRemoteFolder(const nlohmann::json& json);
	static std::unique_ptr<IRemoteObject> CreateRemoteInstance(const nlohmann::json& json);
	static std::unique_ptr<IRemoteObject> CreateRemoteStaticMeshInstance(const nlohmann::json& json);
	static std::unique_ptr<IRemoteObject> CreateRemoteSkinningMeshInstance(const nlohmann::json& json);
	static std::unique_ptr<IRemoteObject> CreateRemoteCamera3DInstance(const nlohmann::json& json);
	static std::unique_ptr<IRemoteObject> CreateRemoteSphereColliderInstance(const nlohmann::json& json);
	static std::unique_ptr<IRemoteObject> CreateRemoteAABBColliderInstance(const nlohmann::json& json);

	//static nlohmann::json SaveRemoteScene(Reference<const RemoteSceneObject> scene);
	//static nlohmann::json SaveRemoteWorld(Reference<const RemoteWorldObject> world);
	//static nlohmann::json SaveRemoteFolder(Reference<const FolderObject> folder);
	//static nlohmann::json SaveRemoteInstance(Reference<const RemoteWorldInstance> instance);
};

#endif // DEBUG_FEATURES_ENABLE
