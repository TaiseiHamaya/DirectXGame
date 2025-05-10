#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include <Library/Utility/Tools/ConstructorMacro.h>

#include "Engine/GraphicsAPI/DirectX/DxResource/TextureResource/TempTexture.h"

class EditorSceneView final {
public:
	EditorSceneView() = default;
	~EditorSceneView() = default;

	__CLASS_NON_COPYABLE(EditorSceneView)

public:
	void initialize(bool isActive_);

	void draw();

	void finalize();

	bool is_hovered_window();

private:
	void copy_screen();
	void set_imgui_command();

private:
	bool isActive{ false };
	bool isHoverMain{ false };
	TempTexture screenResultTexture;
};

#endif // DEBUG_FEATURES_ENABLE
