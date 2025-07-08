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

ImGuiManager& ImGuiManager::GetInstance() noexcept {
	static ImGuiManager instance{};
	return instance;
}

void ImGuiManager::Initialize() {
	// ----------ImGui初期化----------
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	ImGui_ImplWin32_Init(WinApp::GetWndHandle());

	ImGui_ImplDX12_InitInfo initInfo{};
	initInfo.Device = DxDevice::GetDevice().Get();
	initInfo.CommandQueue = DxCommand::GetCommandQueue().Get();
	initInfo.NumFramesInFlight = RenderingSystemValues::NUM_BUFFERING;
	initInfo.RTVFormat = DxSystemValues::SCREEN_RTV_FORMAT;
	initInfo.SrvDescriptorHeap = SRVDescriptorHeap::GetDescriptorHeap().Get();
	initInfo.SrvDescriptorAllocFn = [](ImGui_ImplDX12_InitInfo*, D3D12_CPU_DESCRIPTOR_HANDLE* outHandleCpu, D3D12_GPU_DESCRIPTOR_HANDLE* outHandleGpu) {
		auto srvIndex = SRVDescriptorHeap::UseHeapIndex();
		*outHandleCpu = SRVDescriptorHeap::GetCPUHandle(srvIndex);
		*outHandleGpu = SRVDescriptorHeap::GetGPUHandle(srvIndex);
	};
	initInfo.SrvDescriptorFreeFn = [](ImGui_ImplDX12_InitInfo*, D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle) {
		SRVDescriptorHeap::ReleaseHeapHandle(gpuHandle);
	};
	ImGui_ImplDX12_Init(&initInfo);

	ImFontConfig config{};
	config.MergeMode = true;
	config.GlyphRanges = IconsGlyphRanges;
	config.GlyphOffset.y = 2.f;

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("./DirectXGame/EngineResources/Misc/UDEVGothic35HS-Regular.ttf", 13.f, nullptr, glyphRangesJapanese);
	io.Fonts->AddFontFromFileTTF("./DirectXGame/EditorResources/MaterialSymbolsOutlined[FILL,GRAD,opsz,wght].ttf", 14.5f, &config, IconsGlyphRanges);
	ImGui::GetStyle().Colors[2] = ImVec4{ 0.1f,0.1f,0.1f,1 };
	ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;

	ImGuizmo::Enable(true);
}

void ImGuiManager::Finalize() {
	// ImGui終了処理
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiManager::BeginFrame() {
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
}

void ImGuiManager::EndFrame() {
	Reference<ScreenTexture> screen = DxSwapChain::GetWriteBufferTexture();
	screen->start_write();

	ImGui::Render();
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), DxCommand::GetCommandList().Get());
}

#endif // _DEBUG