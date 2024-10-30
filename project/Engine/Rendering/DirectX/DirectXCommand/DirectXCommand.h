#pragma once

#define NOMINMAX

#include <wrl/client.h>
#include <d3d12.h>

#include <cstdint>
#include <vector>

class DirectXCommand final {
public:
	DirectXCommand() noexcept = default;
	~DirectXCommand() noexcept = default;

private:
	DirectXCommand(const DirectXCommand&) = delete;
	DirectXCommand& operator=(const DirectXCommand&) = delete;

public:
	static void Initialize();

	static DirectXCommand& GetInstance() noexcept;
	static const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& GetCommandList() noexcept;
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
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue;
	Microsoft::WRL::ComPtr<ID3D12Fence> fence;
	std::uint64_t fenceIndex;
	HANDLE fenceEvent;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocatorTexture;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandListTexture;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueueTexture;
	Microsoft::WRL::ComPtr<ID3D12Fence> textureFence;
	std::uint64_t textureFenceIndex;
	HANDLE textureFenceEvent;
};

