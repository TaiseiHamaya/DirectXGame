#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include <Library/Utility/Template/SingletonInterface.h>
//#include <Library/Utility/Template/Reference.h>

#include "Engine/GraphicsAPI/DirectX/DxResource/TextureResource/TempTexture.h"

class EditorMain final : public SingletonInterface<EditorMain> {
	__CLASS_SINGLETON_INTERFACE(EditorMain)

public:
	static void Initialize();

	static void Draw();

	static void Finalize();

	static bool IsHoverEditorWindow();

private:
	void copy_screen();
	void set_imgui_command();

private:
	TempTexture screenResultTexture;
	bool isHoverMain{ false };
	bool isActiveEditor{ true };
};

#endif // DEBUG_FEATURES_ENABLE
