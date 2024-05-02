#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <cstdint>

class DirectXCommand {
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

private:
	void create_command();
	void create_fence();

public:
	void close_and_execute();
	void wait_for_command();
	void reset();

private:
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue;
	Microsoft::WRL::ComPtr<ID3D12Fence> fence;
	uint64_t fenceIndex;
	HANDLE fenceEvent;
};

