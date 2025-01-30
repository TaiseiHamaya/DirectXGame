#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>

class DxDevice final {
private:
	DxDevice() noexcept = default;
	DxDevice(const DxDevice&) noexcept = delete;

public:
	~DxDevice() noexcept = default;

private:
	DxDevice& operator=(const DxDevice&) = delete;

private:
	static DxDevice& GetInstance() noexcept;

public:
	static void Initialize();
	static const Microsoft::WRL::ComPtr<ID3D12Device10>& GetDevice() noexcept { return GetInstance().device; };
	static const Microsoft::WRL::ComPtr<IDXGIFactory7>& GetFactory() noexcept { return GetInstance().dxgiFactory; }

private:
	void create_dxgiFactory();
	void create_adapter();
	void create_device();

private:
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory;
	Microsoft::WRL::ComPtr<IDXGIAdapter4> useAdapter;
	Microsoft::WRL::ComPtr<ID3D12Device10> device;
};

