#pragma once

#include <d3d12.h>
#include <wrl/client.h>

#include <cstdint>
#include <vector>

class DxCommand final {
public:
	DxCommand() noexcept = default;
	~DxCommand() noexcept = default;

private:
	DxCommand(const DxCommand&) = delete;
	DxCommand& operator=(const DxCommand&) = delete;

public:
	static void Initialize();

	static DxCommand& GetInstance() noexcept;
	static const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList4>& GetCommandList() noexcept;
	static const Microsoft::WRL::ComPtr<ID3D12CommandQueue>& GetCommandQueue() noexcept;
	static void SetBarrier(const Microsoft::WRL::ComPtr<ID3D12Resource>& resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);

	// ----------要修正----------
	static void SetTextureCommand(
		const Microsoft::WRL::ComPtr<ID3D12Resource>& resource,
		const Microsoft::WRL::ComPtr<ID3D12Resource>& intermediateResource,
		const std::vector<D3D12_SUBRESOURCE_DATA>& subResources
	);
	static void ExecuteTextureCommand();
	static void WaitTextureCommand();
	static void ResetTextureCommand();
	// ----------要修正----------

private:
	void create_command();
	void create_fence();

public:
	void close_and_kick();
	void wait_command();
	void reset();

private:
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList4> commandList;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue;
	Microsoft::WRL::ComPtr<ID3D12Fence> fence;
	std::uint64_t fenceIndex;
	HANDLE fenceEvent;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocatorTexture;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList4> commandListTexture;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueueTexture;
	Microsoft::WRL::ComPtr<ID3D12Fence> textureFence;
	std::uint64_t textureFenceIndex;
	HANDLE textureFenceEvent;
};

