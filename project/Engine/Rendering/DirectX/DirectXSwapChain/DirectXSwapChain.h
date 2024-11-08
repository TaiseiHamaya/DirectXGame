#pragma once

#include <dxgi1_5.h>
#include <memory>
#include <wrl/client.h>

#include "Engine/Rendering/DirectX/DirectXResourceObject/DepthStencil/DepthStencil.h"
#include "Library/Math/Color.h"

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

public:
	static void SwapScreen();

public:
	//static const Microsoft::WRL::ComPtr<IDXGISwapChain4>& GetSwapChain() noexcept { return GetInstance().swapChain; }
	static const std::shared_ptr<SwapChainRenderTargetGroup>& GetRenderTarget();

	/// <summary>
	/// !!!!!!!!!!!!!!!!!別の場所に置く!!!!!!!!!!!!!!!!!
	/// </summary>
	/// <returns></returns>
	static const std::shared_ptr<DepthStencil>& GetDepthStencil() noexcept;
	static void SetClearColor(const Color& color_) noexcept;
	static void EndRenderTarget();

private:
	static DirectXSwapChain& GetInstance() noexcept;

private:
	void create_swapchain();
	void create_render_terget();
	void swap_screen();

private:
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;
	std::shared_ptr<SwapChainRenderTargetGroup> renderTarget;

	/// <summary>
	/// 修正予定
	/// !!!!!!!!!!!!!!!!!別の場所に置く!!!!!!!!!!!!!!!!!
	/// </summary>
	std::shared_ptr<DepthStencil> depthStencil;
};
