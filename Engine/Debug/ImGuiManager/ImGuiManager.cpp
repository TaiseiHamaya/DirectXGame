#ifdef _DEBUG

#include "ImGuiManager.h"

#include <memory>

#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/DirectX/DirectXDescriptorHeap/SRVDescriptorHeap/SRVDescriptorHeap.h"
#include "Engine/DirectX/DirectXDevice/DirectXDevice.h"
#include "Engine/DirectX/DirectXSwapChain/DirectXSwapChain.h"
#include "Engine/WinApp.h"
#include "Engine/Render/RenderTargetGroup/SwapChainRenderTargetGroup.h"

#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"

ImGuiManager& ImGuiManager::GetInstance() noexcept {
	static ImGuiManager instance{};
	return instance;
}

void ImGuiManager::Initialize() {
	std::uint32_t index = SRVDescriptorHeap::UseHeapIndex();
	// ----------ImGui初期化----------
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	ImGui_ImplWin32_Init(WinApp::GetWndHandle());
	auto format = DirectXSwapChain::GetRenderTarget()->get_render_targets()[0].get_rtv_desc().Format;
	ImGui_ImplDX12_Init(
		DirectXDevice::GetDevice().Get(),
		SWAPCHAIN_HEAP,
		format,
		SRVDescriptorHeap::GetDescriptorHeap().Get(),
		SRVDescriptorHeap::GetCPUHandle(index),
		SRVDescriptorHeap::GetGPUHandle(index)
	);
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
}

void ImGuiManager::EndFrame() {
	ImGui::Render();
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), DirectXCommand::GetCommandList().Get());
}

#endif // _DEBUG