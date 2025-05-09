#ifdef DEBUG_FEATURES_ENABLE

#include "EditorMain.h"

#include <imgui.h>

#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"
#include "Engine/GraphicsAPI/DirectX/DxSwapChain/DxSwapChain.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/TextureResource/ScreenTexture.h"

void EditorMain::Initialize() {
	EditorMain& instance = GetInstance();
	instance.screenResultTexture.initialize();
}

void EditorMain::Draw() {
	EditorMain& instance = GetInstance();
	instance.copy_screen();
	instance.set_imgui_command();
}

void EditorMain::Finalize() {
}

bool EditorMain::IsHoverEditorWindow() {
	EditorMain& instance = GetInstance();
	return 
		(instance.isActiveEditor && instance.isHoverMain) || 
		!ImGui::GetIO().WantCaptureMouse;
}

void EditorMain::copy_screen() {
	auto& instance = DxCommand::GetCommandList();
	Reference<ScreenTexture> screen = DxSwapChain::GetWriteBufferTexture();

	screenResultTexture.start_copy_dest();
	screen->start_read();

	instance->CopyResource(screenResultTexture.get_resource().Get(), screen->get_resource().Get());
}

void EditorMain::set_imgui_command() {
	EditorMain& instance = GetInstance();
	if (isHoverMain && (ImGui::GetIO().MouseDown[1] || ImGui::GetIO().MouseDown[2])) {
		ImGui::SetNextWindowFocus();
	}

	instance.screenResultTexture.start_read();
	ImGui::Begin("EditorMain", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);

	isHoverMain = ImGui::IsWindowHovered();

	ImVec2 winSize = ImGui::GetContentRegionAvail();
	float aspectX = winSize.x / 16;
	float aspectY = winSize.y / 9;

	ImVec2 size = aspectX > aspectY ?
		ImVec2{ winSize.y / 9 * 16, winSize.y } :
		ImVec2{ winSize.x, winSize.x / 16 * 9 };

	ImVec2 prevCursorPos = ImGui::GetCursorPos();
	ImVec2 cursorPos = { prevCursorPos.x + (winSize.x - size.x) * 0.5f, prevCursorPos.y + (winSize.y - size.y) * 0.5f };
	ImGui::SetCursorScreenPos(cursorPos);
	ImGui::Image(
		static_cast<ImTextureID>(instance.screenResultTexture.get_as_srv()->handle().ptr), size
	);
	ImGui::End();
}

#endif // DEBUG_FEATURES_ENABLE