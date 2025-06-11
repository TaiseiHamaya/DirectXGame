#ifdef DEBUG_FEATURES_ENABLE

#include "ImGuiManager.h"

#include "./ImGuiIcons.h"
#include "./ImGuiJapanese.h"
#include "Engine/Application/WinApp.h"
#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"
#include "Engine/GraphicsAPI/DirectX/DxDescriptorHeap/SRVDescriptorHeap/SRVDescriptorHeap.h"
#include "Engine/GraphicsAPI/DirectX/DxDevice/DxDevice.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/TextureResource/ScreenTexture.h"
#include "Engine/GraphicsAPI/DirectX/DxSwapChain/DxSwapChain.h"
#include "Engine/GraphicsAPI/DirectX/DxSystemValues.h"
#include "Engine/GraphicsAPI/RenderingSystemValues.h"

#include <imgui.h>
#include <imgui_impl_dx12.h>
#include <imgui_impl_win32.h>
#include <ImGuizmo.h>


#include <Engine/Runtime/Input/InputHandler.h>

ImGuiManager& ImGuiManager::GetInstance() noexcept {
	static ImGuiManager instance{};
	return instance;
}

void ImGuiManager::Initialize() {
	auto& srvIndex = GetInstance().srvIndex;
	srvIndex = SRVDescriptorHeap::UseHeapIndex();
	// ----------ImGui初期化----------
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	ImGui_ImplWin32_Init(WinApp::GetWndHandle());
	ImGui_ImplDX12_Init(
		DxDevice::GetDevice().Get(),
		RenderingSystemValues::NUM_BUFFERING,
		DxSystemValues::SCREEN_RTV_FORMAT,
		SRVDescriptorHeap::GetDescriptorHeap().Get(),
		SRVDescriptorHeap::GetCPUHandle(srvIndex),
		SRVDescriptorHeap::GetGPUHandle(srvIndex)
	);

	ImFontConfig config{};
	config.MergeMode = true;
	config.GlyphRanges = IconsGlyphRanges;

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("./DirectXGame/EngineResources/Misc/UDEVGothic35HS-Regular.ttf", 13.f, nullptr, glyphRangesJapanese);
	io.Fonts->AddFontFromFileTTF("./DirectXGame/EditorResources/MaterialSymbolsOutlined[FILL,GRAD,opsz,wght].ttf", 13.f, &config, IconsGlyphRanges);
	ImGui::GetStyle().Colors[2] = ImVec4{ 0.1f,0.1f,0.1f,1 };
	ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;

	ImGuizmo::Enable(true);
}

void ImGuiManager::Finalize() {
	auto& srvIndex = GetInstance().srvIndex;

	// ImGui終了処理
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	SRVDescriptorHeap::ReleaseHeapIndex(srvIndex);
}

void ImGuiManager::BeginFrame() {
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	static InputHandler<KeyID> input;
	static std::once_flag flag;
	std::call_once(flag, [&]() {input.initialize({ KeyID::F6 }); });
	static bool isActiveGizmo{ true };
	input.update();
	if (input.trigger(KeyID::F6)) {
		isActiveGizmo ^= 1;
	}
	if (isActiveGizmo) {
		ImGuizmo::BeginFrame();
	}
}

void ImGuiManager::EndFrame() {
	Reference<ScreenTexture> screen = DxSwapChain::GetWriteBufferTexture();
	screen->start_write();

	ImGui::Render();
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), DxCommand::GetCommandList().Get());
}

#endif // _DEBUG