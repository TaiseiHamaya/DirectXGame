#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "IEditorWindow.h"

#include "Engine/GraphicsAPI/DirectX/DxResource/TextureResource/TempTexture.h"

#include <Library/Math/Vector2.h>

class EditorSceneView final : public IEditorWindow {
public:
	EditorSceneView() = default;
	~EditorSceneView() = default;

	__CLASS_NON_COPYABLE(EditorSceneView)

public:
	void initialize(bool isActive_);

	void draw() override;

	bool is_hovered_window();

	const Vector2& view_origin() const;
	const Vector2& view_size() const;

private:
	void copy_screen();
	void set_imgui_command();

private:
	bool isHoverWindow{ false };
	TempTexture screenResultTexture;

	Vector2 origin;
	Vector2 size;
};

#endif // DEBUG_FEATURES_ENABLE
