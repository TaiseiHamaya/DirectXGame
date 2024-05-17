#pragma once

#include <dxgi1_6.h>
#include <wrl.h>
#include <array>
#include <memory>

#include "Engine/DirectX/DirectXResourceObject/RenderTarget/RenderTarget.h"
#include "Engine/DirectX/DirectXResourceObject/DepthStencil/DepthStencil.h"

// ダブルバッファなのでHeapも2
constexpr uint32_t SWAPCHAIN_HEAP = 2;

class DirectXSwapChain {
private:
	DirectXSwapChain();

public:
	~DirectXSwapChain() = default;

private:
	DirectXSwapChain(const DirectXSwapChain&) = delete;
	DirectXSwapChain& operator=(const DirectXSwapChain&) = delete;

public:
	static void Initialize();
	static const Microsoft::WRL::ComPtr<IDXGISwapChain4>& GetSwapChain() { return GetInstance().swapChain; }
	static UINT GetBackBufferIndex() { return GetInstance().backBufferIndex; }

	static void SetRenderTarget();
	static void SwapScreen();
	static void ChangeBackBufferState();
	static void ClearScreen();
	static void ClearDepthStencil();

	static const DepthStencil& GetDepthStencil() { return *GetInstance().depthStencil; };

private:
	static DirectXSwapChain& GetInstance();

private:
	void create_swapchain();
	void change_back_buffer_state();
	void swap_screen();

private:
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;
	bool isRendering;
	UINT backBufferIndex;
	std::array<RenderTarget, SWAPCHAIN_HEAP> renderTarget;
	std::unique_ptr<DepthStencil> depthStencil;
};

