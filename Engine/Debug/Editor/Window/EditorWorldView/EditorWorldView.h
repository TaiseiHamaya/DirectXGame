#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include <memory>

#include <Library/Utility/Template/Reference.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

#include "EditorDebugCamera.h"

class RemoteWorldObject;
class DirectionalLightInstance;
class StaticMeshInstance;
class TempTexture;
class PrimitiveGeometryDrawExecutor;

class EditorWorldView final {
public:
	EditorWorldView() = default;
	~EditorWorldView() = default;

	__CLASS_NON_COPYABLE(EditorWorldView)

public:
	void initialize();
	void setup(Reference<RemoteWorldObject> remoteWorld_);

	void register_mesh(Reference<StaticMeshInstance> meshInstance);
	void register_primitive(const std::string& name, const Affine& affine);

	void update();
	void set_camera_command();
	void draw_lines();
	std::tuple<bool, Vector2, Vector2> draw_editor(const TempTexture& texture);

	void camera_gui();
	Reference<const Camera3D> get_camera() const;

	bool is_select_tab() const;
	Reference<RemoteWorldObject> remote_world() const;

private:
	bool isSelectTab{ false };
	Reference<RemoteWorldObject> remoteWorld;

	std::unique_ptr<EditorDebugCamera> cameraInstance;
	std::unordered_map<std::string, std::unique_ptr<PrimitiveGeometryDrawExecutor>> primitive;
};

#endif // DEBUG_FEATURES_ENABLE
