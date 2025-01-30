#ifdef _DEBUG

#include "ImGuiManager.h"

#include "Engine/Application/WinApp.h"
#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"
#include "Engine/GraphicsAPI/DirectX/DxDescriptorHeap/SRVDescriptorHeap/SRVDescriptorHeap.h"
#include "Engine/GraphicsAPI/DirectX/DxDevice/DxDevice.h"
#include "Engine/GraphicsAPI/DirectX/DxSystemValues.h"
#include "Engine/Module/Render/RenderTargetGroup/SwapChainRenderTargetGroup.h"

#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"

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
}

void ImGuiManager::EndFrame() {
	ImGui::Render();
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), DxCommand::GetCommandList().Get());
}

#endif // _DEBUG