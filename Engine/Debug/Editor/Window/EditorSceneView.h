#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "IEditorWindow.h"

#include <unordered_map>

#include "EditorWorldView/EditorWorldView.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/TextureResource/TempTexture.h"
#include "Engine/Module/DrawExecutor/LightingExecutor/DirectionalLightingExecutor.h"
#include "Engine/Module/DrawExecutor/Mesh/StaticMeshDrawManager.h"
#include "Engine/Module/Render/RenderPath/RenderPath.h"

#include <Library/Math/Vector2.h>

struct ImDrawList;
class EditorGizmo;
class EditorHierarchy;

class EditorSceneView final : public IEditorWindow {
private:
	struct LayerAccessor {
		u32 layer;
		EditorWorldView view;
	};

public:
	EditorSceneView() = default;
	~EditorSceneView() = default;

	__CLASS_NON_COPYABLE(EditorSceneView)

public:
	void initialize(bool isActive_);
	void setup(Reference<EditorGizmo> gizmo_, Reference<const EditorHierarchy> hierarchy_);

	void update();
	void draw_scene();
	void draw() override;

	bool is_hovered_window();

	const Vector2& view_origin() const;
	const Vector2& view_size() const;

	Reference<ImDrawList> draw_list() const;

	void register_world(Reference<RemoteWorldObject> world);

	void register_mesh(Reference<const RemoteWorldObject> world, Reference<const StaticMeshInstance> instance);
	void write_primitive(Reference<const RemoteWorldObject> world, const std::string& primitiveName, const Affine& affine);

	std::optional<u32> get_layer(Reference<const RemoteWorldObject> world) const;

	Reference<EditorWorldView> get_world_view(Reference<const RemoteWorldObject> worldRef);
	Reference<EditorWorldView> get_current_world_view();

private:
	void copy_screen();
	void set_imgui_command();

private:
	bool isHoverWindow{ false };
	u32 layerSize;

	Vector2 origin;
	Vector2 size;

	Reference<EditorGizmo> gizmo;
	Reference<const EditorHierarchy> hierarchy;
	Reference<ImDrawList> drawList;

	Reference<RemoteWorldObject> selectWorldObject;

	std::unordered_map<Reference<const RemoteWorldObject>, LayerAccessor> worldViews;

	// 描画用データ
	TempTexture screenResultTexture;
	RenderPath renderPath;
	std::unique_ptr<DirectionalLightInstance> lightInstance;
	DirectionalLightingExecutor directionalLightingExecutor;
	StaticMeshDrawManager staticMeshDrawManager;
};

#endif // DEBUG_FEATURES_ENABLE
