#include "DxCommand.h"

#include <cassert>

#include "Engine/GraphicsAPI/DirectX/DxDevice/DxDevice.h"

#include <d3dx12.h>

void DxCommand::Initialize() {
	GetInstance();
	GetInstance().create_command();
	GetInstance().create_fence();
}

const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList4>& DxCommand::GetCommandList() noexcept {
	return GetInstance().commandList;
}

const Microsoft::WRL::ComPtr<ID3D12CommandQueue>& DxCommand::GetCommandQueue() noexcept {
	return GetInstance().commandQueue;
}

void DxCommand::SetBarrier(const Microsoft::WRL::ComPtr<ID3D12Resource>& resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after) {
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = resource.Get();
	barrier.Transition.StateBefore = before;
	barrier.Transition.StateAfter = after;
	DxCommand::GetCommandList()->ResourceBarrier(1, &barrier); // バリアの適用
}

// ----------------------ここから後で直す----------------------
void DxCommand::SetTextureCommand(const Microsoft::WRL::ComPtr<ID3D12Resource>& resource, const Microsoft::WRL::ComPtr<ID3D12Resource>& intermediateResource, const std::vector<D3D12_SUBRESOURCE_DATA>& subResources) {
	UpdateSubresources(GetInstance().commandListTexture.Get(), resource.Get(), intermediateResource.Get(), 0, 0, UINT(subResources.size()), subResources.data()); // コマンドに積む

	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = resource.Get();
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	GetInstance().commandListTexture->ResourceBarrier(1, &barrier);
}

void DxCommand::ExecuteTextureCommand() {
	HRESULT hr;
	ID3D12GraphicsCommandList* commandList = GetInstance().commandListTexture.Get();
	// コマンドリストのクローズ
	hr = commandList->Close();
	assert(SUCCEEDED(hr)); // 失敗したら停止させる
	// まとめる
	ID3D12CommandList* commandLists[] = { commandList };
	// キック
	GetInstance().commandQueueTexture->ExecuteCommandLists(_countof(commandLists), commandLists);
}

void DxCommand::WaitTextureCommand() {
	// ----------シグナルの発行----------
	const std::uint64_t& fenceIndex = ++GetInstance().textureFenceIndex;
	Microsoft::WRL::ComPtr<ID3D12Fence>& fence = GetInstance().textureFence;
	HANDLE& fenceEvent = GetInstance().textureFenceEvent;
	GetInstance().commandQueueTexture->Signal(fence.Get(), fenceIndex);
	// ----------シグナルまで到達してるか----------
	if (fence->GetCompletedValue() < fenceIndex) {
		fence->SetEventOnCompletion(fenceIndex, fenceEvent);
		// 終わるまで待つ
		WaitForSingleObject(fenceEvent, INFINITE);
	}
}

void DxCommand::ResetTextureCommand() {
	HRESULT hr;
	// ----------リセット----------
	hr = GetInstance().commandAllocatorTexture->Reset();
	assert(SUCCEEDED(hr)); // 失敗したら停止させる
	hr = GetInstance().commandListTexture->Reset(GetInstance().commandAllocatorTexture.Get(), nullptr);
	assert(SUCCEEDED(hr)); // 失敗したら停止させる
}
// ----------------------ここまで後で直す----------------------

DxCommand& DxCommand::GetInstance() noexcept {
	static DxCommand instance{};
	return instance;
}

void DxCommand::create_command() {
	const Microsoft::WRL::ComPtr<ID3D12Device>& device = DxDevice::GetDevice();
	HRESULT hr;
	// ----------コマンドキューの生成----------
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	hr = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(commandQueue.GetAddressOf()));
	// 失敗したら停止させる
	assert(SUCCEEDED(hr));

	// ----------コマンドアロケータの生成----------
	hr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(commandAllocator.GetAddressOf()));
	// 失敗したら停止させる
	assert(SUCCEEDED(hr));

	// ----------コマンドリストを生成する----------
	hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr, IID_PPV_ARGS(commandList.GetAddressOf()));
	// 失敗したら停止させる
	assert(SUCCEEDED(hr));

	// ----------コマンドキューの生成----------
	hr = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(commandQueueTexture.GetAddressOf()));
	// 失敗したら停止させる
	assert(SUCCEEDED(hr));

	// ----------テクスチャ用コマンドアロケータの生成----------
	hr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocatorTexture));
	// 失敗したら停止させる
	assert(SUCCEEDED(hr));

	// ----------テクスチャ用コマンドリストを生成する----------
	hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocatorTexture.Get(), nullptr, IID_PPV_ARGS(commandListTexture.GetAddressOf()));
	// 失敗したら停止させる
	assert(SUCCEEDED(hr));
}

void DxCommand::create_fence() {
	HRESULT hr;
	// ----------フェンスの生成----------
	uint64_t fenceValue = 0;
	hr = DxDevice::GetDevice()->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	assert(SUCCEEDED(hr)); // 失敗したら停止させる
	hr = DxDevice::GetDevice()->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&textureFence));
	assert(SUCCEEDED(hr)); // 失敗したら停止させる
	HANDLE fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent != nullptr);
	HANDLE textureFenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(textureFenceEvent != nullptr);
	fenceIndex = 0;
	textureFenceIndex = 0;
}

void DxCommand::close_and_kick() {
	HRESULT hr;
	// コマンドリストのクローズ
	hr = commandList->Close();
	assert(SUCCEEDED(hr)); // 失敗したら停止させる
	// まとめる
	ID3D12CommandList* commandLists[] = { commandList.Get() };
	// キック
	commandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);
}

void DxCommand::wait_command() {
	// ----------シグナルの発行----------
	++fenceIndex;
	commandQueue->Signal(fence.Get(), fenceIndex);
	// ----------シグナルまで到達してるか----------
	if (fence->GetCompletedValue() < fenceIndex) {
		fence->SetEventOnCompletion(fenceIndex, fenceEvent);
		// 終わるまで待つ
		WaitForSingleObject(fenceEvent, INFINITE);
	}
}

void DxCommand::reset() {
	HRESULT hr;
	// ----------リセット----------
	hr = commandAllocator->Reset();
	assert(SUCCEEDED(hr)); // 失敗したら停止させる
	hr = commandList->Reset(commandAllocator.Get(), nullptr);
	assert(SUCCEEDED(hr)); // 失敗したら停止させる
}
