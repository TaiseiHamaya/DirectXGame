#pragma once

#include <dxgi1_5.h>
#include <memory>
#include <wrl/client.h>

#include "Library/Math/Color4.h"

class SwapChainRenderTargetGroup;
class PSOBuilder;

class DirectXSwapChain final {
private:
	DirectXSwapChain() noexcept = default;

public:
	~DirectXSwapChain() noexcept = default;

private:
	DirectXSwapChain(const DirectXSwapChain&) = delete;
	DirectXSwapChain& operator=(const DirectXSwapChain&) = delete;

public:
	static void Initialize();
	static void Finalize();

public:
	static void SwapScreen();

public:
	//static const Microsoft::WRL::ComPtr<IDXGISwapChain4>& GetSwapChain() noexcept { return GetInstance().swapChain; }
	static const std::shared_ptr<SwapChainRenderTargetGroup>& GetRenderTarget();

	static void SetClearColor(const Color4& color_) noexcept;
	static void EndRenderTarget();

private:
	static DirectXSwapChain& GetInstance() noexcept;

private:
	void create_swapchain();
	void create_render_target();
	void swap_screen();

private:
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;
	std::shared_ptr<SwapChainRenderTargetGroup> renderTarget;
};
