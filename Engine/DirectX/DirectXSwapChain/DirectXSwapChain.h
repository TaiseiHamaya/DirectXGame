#pragma once

#include <dxgi1_6.h>
#include <wrl.h>

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
	static const Microsoft::WRL::ComPtr<IDXGISwapChain4>& GetSwapChain() { return GetInstance().swapChain; };
	static void ChangeBackBufferState();

private:
	static DirectXSwapChain& GetInstance();

private:
	void create_swapchain(const HWND& hWnd);
	void change_back_buffer_state();

private:
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;
	bool is_rendering;
};

