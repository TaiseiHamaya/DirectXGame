#ifdef _DEBUG

#include "ImGuiManager.h"

#include <memory>

#include "Engine/WinApp.h"
#include "Engine/DirectX/DirectXDevice/DirectXDevice.h"
#include "Engine/DirectX/DirectXSwapChain/DirectXSwapChain.h"
#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/DirectX/DirectXDescriptorHeap/SRVDescriptorHeap/SRVDescriptorHeap.h"

#include "externals/imgui/imgui.h"
#include "externals/imgui/imgui_impl_dx12.h"
#include "externals/imgui/imgui_impl_win32.h"

ImGuiManager& ImGuiManager::GetInstance() {
	static std::unique_ptr<ImGuiManager> instance{ new ImGuiManager };
	return *instance;
}

void ImGuiManager::Initialize() {
	std::uint32_t index = SRVDescriptorHeap::GetNextHandleIndex();
	// ----------ImGui初期化----------
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	ImGui_ImplWin32_Init(WinApp::GetWndHandle());
	ImGui_ImplDX12_Init(
		DirectXDevice::GetDevice().Get(),
		SWAPCHAIN_HEAP,
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		SRVDescriptorHeap::GetDescriptorHeap().Get(),
		SRVDescriptorHeap::GetCPUHandle(index),
		SRVDescriptorHeap::GetGPUHandle(index)
	);
	//ImGui::UpdatePlatformWindows();
}

void ImGuiManager::Finalize() {
	// ImGui終了処理
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiManager::BeginFrame() {
	//ImGui::UpdatePlatformWindows();
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGuiManager::EndFrame() {
	ImGui::Render();
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), DirectXCommand::GetCommandList().Get());
}

#endif // _DEBUG