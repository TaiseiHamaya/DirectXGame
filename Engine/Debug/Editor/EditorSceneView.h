#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "IEditorWindow.h"

#include "Engine/GraphicsAPI/DirectX/DxResource/TextureResource/TempTexture.h"

#include <Library/Math/Vector2.h>

struct ImDrawList;
class EditorGizmo;

class EditorSceneView final : public IEditorWindow {
public:
	EditorSceneView() = default;
	~EditorSceneView() = default;

	__CLASS_NON_COPYABLE(EditorSceneView)

public:
	void initialize(bool isActive_);
	void setup(Reference<EditorGizmo> gizmo_);

	void draw() override;

	bool is_hovered_window();

	const Vector2& view_origin() const;
	const Vector2& view_size() const;

	Reference<ImDrawList> draw_list() const;

private:
	void copy_screen();
	void set_imgui_command();

private:
	bool isHoverWindow{ false };
	TempTexture screenResultTexture;

	Vector2 origin;
	Vector2 size;

	Reference<EditorGizmo> gizmo;
	Reference<ImDrawList> drawList;
};

#endif // DEBUG_FEATURES_ENABLE
