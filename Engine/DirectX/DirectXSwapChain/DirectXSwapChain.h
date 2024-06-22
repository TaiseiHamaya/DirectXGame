#pragma once

#include <array>
#include <dxgi1_5.h>
#include <memory>
#include <wrl/client.h>

#include "Engine/DirectX/DirectXResourceObject/RenderTarget/RenderTarget.h"
#include "Engine/DirectX/DirectXResourceObject/DepthStencil/DepthStencil.h"
#include "Engine/Math/Color.h"

class SwapChainRenderNode;
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
	static void SetRenderTarget();
	static void SwapScreen();
	static void ChangeBackBufferState();
	static void SetPSOFromBuilder(const std::unique_ptr<PSOBuilder>& psoBuilder);

public:
	static const Microsoft::WRL::ComPtr<IDXGISwapChain4>& GetSwapChain() noexcept { return GetInstance().swapChain; }
	static UINT GetBackBufferIndex() noexcept { return GetInstance().backBufferIndex; }
	static const DepthStencil& GetDepthStencil() noexcept;
	static void SetClearColor(const Color& color_) noexcept;

private:
	static DirectXSwapChain& GetInstance() noexcept;

private:
	void create_swapchain();
	void create_render_terget_view();
	void create_render_node();
	void swap_screen();

private:
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;
	UINT backBufferIndex;
	std::array<RenderTarget, SWAPCHAIN_HEAP> renderTarget;
	std::unique_ptr<SwapChainRenderNode> renderNode;
};
