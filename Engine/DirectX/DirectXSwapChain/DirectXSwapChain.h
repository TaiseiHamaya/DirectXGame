#pragma once

#include <dxgi1_6.h>
#include <wrl.h>
#include <array>

#include "Engine/DirectX/DirectXResourceObject/RenderTarget/RenderTarget.h"

// ダブルバッファなのでHeapも2
constexpr uint32_t HEAPSIZE = 2;

class DirectXSwapChain {
private:
	DirectXSwapChain();

public:
	~DirectXSwapChain() = default;

private:
	DirectXSwapChain(const DirectXSwapChain&) = delete;
	DirectXSwapChain& operator=(const DirectXSwapChain&) = delete;

public:
	static void Initialize(const HWND& hWnd);
	static const Microsoft::WRL::ComPtr<IDXGISwapChain4>& GetSwapChain() { return GetInstance().swapChain; }
	static UINT GetBackBufferIndex() { return GetInstance().backBufferIndex; }

	static void SetRenderTarget();
	static void SwapScreen();
	static void ChangeBackBufferState();
	static void ClearScreen();

private:
	static DirectXSwapChain& GetInstance();

private:
	void create_swapchain(const HWND& hWnd);
	void change_back_buffer_state();
	void swap_screen();

private:
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;
	bool isRendering;
	UINT backBufferIndex;
	std::array<RenderTarget, HEAPSIZE> renderTarget;
};

