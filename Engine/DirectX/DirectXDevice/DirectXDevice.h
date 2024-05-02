#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>

class DirectXDevice {
private:
	DirectXDevice() = default;
	DirectXDevice(const DirectXDevice&) = delete;

public:
	~DirectXDevice() = default;

private:
	DirectXDevice& operator=(const DirectXDevice&) = delete;

private:
	static DirectXDevice& GetInstance();

public:
	static void Initialize();
	static const Microsoft::WRL::ComPtr<ID3D12Device>& GetDevice() { return GetInstance().device; };
	static const Microsoft::WRL::ComPtr<IDXGIFactory7>& GetFactory() { return GetInstance().dxgiFactory; }

private:
	void create_dxgiFactory();
	void create_adapter();
	void create_device();

private:
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory;
	Microsoft::WRL::ComPtr<IDXGIAdapter4> useAdapter;
	Microsoft::WRL::ComPtr<ID3D12Device> device;
};

