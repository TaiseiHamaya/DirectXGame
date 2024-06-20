#pragma once

#include <array>
#include <dxgi1_5.h>
#include <memory>
#include <wrl/client.h>

#include "Engine/DirectX/DirectXResourceObject/DepthStencil/DepthStencil.h"
#include "Engine/DirectX/DirectXResourceObject/RenderTarget/RenderTarget.h"
#include "Engine/Math/Color.h"

class OffscreenRender;

// ダブルバッファなのでHeapも2
constexpr uint32_t SWAPCHAIN_HEAP = 2;

class DirectXSwapChain final {
private:
	DirectXSwapChain() noexcept;

public:
	~DirectXSwapChain() noexcept = default;

private:
	DirectXSwapChain(const DirectXSwapChain&) = delete;
	DirectXSwapChain& operator=(const DirectXSwapChain&) = delete;

public:
	static void Initialize();

public:
	static void SetOffscreenRenderTarget();
	static void SetOnscreenRenderTarget();
	static void RenderingOffscreen();
	static void SwapScreen();
	static void ChangeBackBufferState();
	static void ChangeOffscreenState();
	static void ClearScreen();
	static void ClearDepthStencil();

public:
	static const Microsoft::WRL::ComPtr<IDXGISwapChain4>& GetSwapChain() noexcept { return GetInstance().swapChain; }
	static UINT GetBackBufferIndex() noexcept { return GetInstance().backBufferIndex; }
	static const DepthStencil& GetDepthStencil() noexcept { return *GetInstance().depthStencil; };
	static void SetClearColor(const Color& color_) noexcept;

private:
	static DirectXSwapChain& GetInstance() noexcept;

private:
	void create_swapchain();
	void create_offscreen();
	void change_back_buffer_state();
	void change_offscreen_state();
	void swap_screen();

private:
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;
	UINT backBufferIndex;
	std::array<RenderTarget, SWAPCHAIN_HEAP> renderTarget;
	std::unique_ptr<OffscreenRender> offscreen;
	std::unique_ptr<DepthStencil> depthStencil;
	Color clearColor;
};

