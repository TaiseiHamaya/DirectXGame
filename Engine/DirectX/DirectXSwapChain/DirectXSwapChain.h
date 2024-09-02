#pragma once

#include <dxgi1_5.h>
#include <memory>
#include <wrl/client.h>

#include "Engine/DirectX/DirectXResourceObject/RenderTarget/RenderTarget.h"
#include "Engine/DirectX/DirectXResourceObject/DepthStencil/DepthStencil.h"
#include "Engine/Game/Color/Color.h"

class SwapChainRenderTargetGroup;
class PSOBuilder;

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
	static void SwapScreen();

public:
	static const Microsoft::WRL::ComPtr<IDXGISwapChain4>& GetSwapChain() noexcept { return GetInstance().swapChain; }
	static UINT GetBackBufferIndex() noexcept { return GetInstance().backBufferIndex; }
	static const std::shared_ptr<SwapChainRenderTargetGroup>& GetRenderTarget();
	static const std::shared_ptr<DepthStencil>& GetDepthStencil() noexcept;
	static void SetClearColor(const Color& color_) noexcept;
	static void ChangeBackBufferState();

private:
	static DirectXSwapChain& GetInstance() noexcept;

private:
	void create_swapchain();
	void create_render_terget();
	void swap_screen();

private:
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;
	UINT backBufferIndex;
	std::shared_ptr<SwapChainRenderTargetGroup> renderTarget;
};
