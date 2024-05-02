#include "DirectXCommand.h"

#include <memory>
#include <cassert>

#include "Engine/DirectX/DirectXDevice/DirectXDevice.h"
#include "Engine/DirectX/DirectXSwapChain/DirectXSwapChain.h"

void DirectXCommand::Initialize() {
	GetInstance();
	GetInstance().create_command();
	GetInstance().create_fence();
}

const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& DirectXCommand::GetCommandList() {
	return GetInstance().commandList;
}

const Microsoft::WRL::ComPtr<ID3D12CommandQueue>& DirectXCommand::GetCommandQueue() {
	return GetInstance().commandQueue;
}

void DirectXCommand::SetBarrier(ID3D12Resource* const resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after) {
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = resource;
	barrier.Transition.StateBefore = before;
	barrier.Transition.StateAfter = after;
	DirectXCommand::GetCommandList()->ResourceBarrier(1, &barrier); // バリアの適用
}

DirectXCommand& DirectXCommand::GetInstance() {
	static std::unique_ptr<DirectXCommand> instance{ new DirectXCommand };
	return *instance;
}

void DirectXCommand::create_command() {
	const Microsoft::WRL::ComPtr<ID3D12Device>& device = DirectXDevice::GetDevice();
	// ----------コマンドキューの生成----------
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	HRESULT hr = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue));
	// 失敗したら停止させる
	assert(SUCCEEDED(hr));

	// ----------コマンドアロケータの生成----------
	hr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
	// 失敗したら停止させる
	assert(SUCCEEDED(hr));

	// ----------コマンドリストを生成する----------
	hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList));
	// 失敗したら停止させる
	assert(SUCCEEDED(hr));
}

void DirectXCommand::create_fence() {
	HRESULT hr;
	// ----------フェンスの生成----------
	uint64_t fenceValue = 0;
	hr = DirectXDevice::GetDevice()->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	assert(SUCCEEDED(hr)); // 失敗したら停止させる
	HANDLE fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent != nullptr);
}

void DirectXCommand::close_and_execute() {
	HRESULT hr;
	// コマンドリストのクローズ
	hr = commandList->Close();
	assert(SUCCEEDED(hr)); // 失敗したら停止させる
	// まとめる
	ID3D12CommandList* commandLists[] = { commandList.Get() };
	// エクスキュート
	commandQueue->ExecuteCommandLists(1, commandLists);
}

void DirectXCommand::wait_for_command() {
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

void DirectXCommand::reset() {
	HRESULT hr;
	// ----------リセット----------
	hr = commandAllocator->Reset();
	assert(SUCCEEDED(hr)); // 失敗したら停止させる
	hr = commandList->Reset(commandAllocator.Get(), nullptr);
	assert(SUCCEEDED(hr)); // 失敗したら停止させる
}
