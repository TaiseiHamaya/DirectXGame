#pragma once

#include <filesystem>

#include <Library/Utility/Template/Reference.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

#include "Engine/Runtime/Scene/SceneAssetCollection.h"

class RemoteSceneObject;
class EditorRenderDAG;

class EditorAssetSaver final {
public:
	EditorAssetSaver() = default;
	~EditorAssetSaver() = default;

	__CLASS_NON_COPYABLE(EditorAssetSaver)

public:
	void setup(Reference<const EditorRenderDAG> dagEditor_, Reference<const RemoteSceneObject> scene_);

	/// <summary>
	/// "filePath / Assets.json"に出力
	/// </summary>
	/// <param name="filePath"></param>
	void save(const std::filesystem::path& filePath);

private:
	void collect_shaders();

	void save_to_json(const std::filesystem::path& filePath);

private:
	Reference<const EditorRenderDAG> dagEditor;
	Reference<const RemoteSceneObject> scene;

	SceneAssetCollection sceneAssetCollection;
};
