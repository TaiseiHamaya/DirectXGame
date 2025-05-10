#ifdef DEBUG_FEATURES_ENABLE

#include "EditorSceneView.h"

#include <imgui.h>

#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/TextureResource/ScreenTexture.h"
#include "Engine/GraphicsAPI/DirectX/DxSwapChain/DxSwapChain.h"

void EditorSceneView::initialize(bool isActive_) {
	isActive = isActive_;
	screenResultTexture.initialize();
}

void EditorSceneView::draw() {
	copy_screen();
	set_imgui_command();
}

void EditorSceneView::finalize() {
}

bool EditorSceneView::is_hovered_window() {
	return
		(isActive && isHoverMain) ||
		!ImGui::GetIO().WantCaptureMouse;
}

void EditorSceneView::copy_screen() {
	auto& instance = DxCommand::GetCommandList();
	Reference<ScreenTexture> screen = DxSwapChain::GetWriteBufferTexture();

	screenResultTexture.start_copy_dest();
	screen->start_read();

	instance->CopyResource(screenResultTexture.get_resource().Get(), screen->get_resource().Get());
}

void EditorSceneView::set_imgui_command() {
	if (isHoverMain && (ImGui::GetIO().MouseDown[1] || ImGui::GetIO().MouseDown[2])) {
		ImGui::SetNextWindowFocus();
	}

	screenResultTexture.start_read();
	ImGui::Begin("Scene", &isActive, ImGuiWindowFlags_NoScrollbar);

	isHoverMain = ImGui::IsWindowHovered();

	ImVec2 winSize = ImGui::GetContentRegionAvail();
	float aspectX = winSize.x / 16;
	float aspectY = winSize.y / 9;

	ImVec2 size = aspectX > aspectY ?
		ImVec2{ winSize.y / 9 * 16, winSize.y } :
		ImVec2{ winSize.x, winSize.x / 16 * 9 };

	ImVec2 cursorPos = { (winSize.x - size.x) * 0.5f, (winSize.y - size.y) * 0.5f };
	ImGui::SetCursorScreenPos(cursorPos);
	ImGui::Image(
		static_cast<ImTextureID>(screenResultTexture.get_as_srv()->handle().ptr), size
	);
	ImGui::End();
}

#endif // DEBUG_FEATURES_ENABLE
