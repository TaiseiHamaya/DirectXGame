#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "IEditorWindow.h"

#include <unordered_map>

#include "EditorWorldView/EditorWorldView.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/TextureResource/TempTexture.h"

#include <Library/Math/Vector2.h>

struct ImDrawList;
class EditorGizmo;
class EditorHierarchy;

class EditorSceneView final : public IEditorWindow {
public:
	EditorSceneView() = default;
	~EditorSceneView() = default;

	__CLASS_NON_COPYABLE(EditorSceneView)

public:
	void initialize(bool isActive_);
	void setup(Reference<EditorGizmo> gizmo_, Reference<const EditorHierarchy> hierarchy_);

	void draw_scene();
	void draw() override;

	bool is_hovered_window();

	const Vector2& view_origin() const;
	const Vector2& view_size() const;

	Reference<ImDrawList> draw_list() const;

	void check_world(Reference<RemoteWorldObject> worldRef);
	Reference<EditorWorldView> get_world_view(Reference<const RemoteWorldObject> worldRef);
	Reference<EditorWorldView> get_current_world_view();

private:
	void copy_screen();
	void set_imgui_command();

private:
	bool isHoverWindow{ false };
	TempTexture screenResultTexture;

	Vector2 origin;
	Vector2 size;

	Reference<EditorGizmo> gizmo;
	Reference<const EditorHierarchy> hierarchy;
	Reference<ImDrawList> drawList;

	Reference<RemoteWorldObject> selectWorldObject;
	std::unordered_map<Reference<const RemoteWorldObject>, EditorWorldView> worldView;
};

#endif // DEBUG_FEATURES_ENABLE
