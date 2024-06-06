#pragma once

#include <wrl/client.h>
#include <d3d12.h>

#include <cstdint>
#include <vector>

class DirectXCommand final {
private:
	DirectXCommand() = default;

public:
	~DirectXCommand() = default;

private:
	DirectXCommand(const DirectXCommand&) = delete;
	DirectXCommand& operator=(const DirectXCommand&) = delete;

public:
	static void Initialize();

	static DirectXCommand& GetInstance();
	static const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& GetCommandList();
	static const Microsoft::WRL::ComPtr<ID3D12CommandQueue>& GetCommandQueue();
	static void SetBarrier(ID3D12Resource* const resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);

	static void SetTextureCommand(
		const Microsoft::WRL::ComPtr<ID3D12Resource>& resource,
		const Microsoft::WRL::ComPtr<ID3D12Resource>& intermediateResource,
		const std::vector<D3D12_SUBRESOURCE_DATA>& subResources
	);
	static void ExecuteTextureCommand();
	static void WaitTextureCommand();
	static void ResetTextureCommand();

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

