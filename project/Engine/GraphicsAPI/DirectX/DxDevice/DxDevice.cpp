#include "DxDevice.h"

#include <cassert>

#include "Engine/Debug/Output.h"

DxDevice& DxDevice::GetInstance() noexcept {
	static DxDevice instance{ };
	return instance;
}

void DxDevice::Initialize() {
	GetInstance();
	// DxgiFactory生成
	GetInstance().create_dxgiFactory();
	// Adapter探しの旅
	GetInstance().create_adapter();
	// Device生成
	GetInstance().create_device();
	Console("Complete create D3D12Device\n");
}

void DxDevice::create_dxgiFactory() {
	// ----------DXGIファクトリーの生成----------
	HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	assert(SUCCEEDED(hr));
}

void DxDevice::create_adapter() {
	// ----------使用するアダプタを検索----------
// 良い順にアダプタを頼む
	for (UINT i = 0; dxgiFactory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&useAdapter)) != DXGI_ERROR_NOT_FOUND; ++i) {
		DXGI_ADAPTER_DESC3 adapterDesc{};
		HRESULT hr = useAdapter->GetDesc3(&adapterDesc);
		assert(SUCCEEDED(hr));
		// ソフトウェアアダプターでなければ採用する
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			// ログ出力
			Console(L"Use Adapter : {}\n", adapterDesc.Description);
			break;
		}
		// ソフトウェアアダプタの場合は無視
		useAdapter = nullptr;
	}
	// 見つからなかったら起動しない
	assert(useAdapter != nullptr);
}

void DxDevice::create_device() {
	// ----------デバイスの生成----------
// 機能レベル一覧
	D3D_FEATURE_LEVEL featureLevel[] = {
		D3D_FEATURE_LEVEL_12_2,D3D_FEATURE_LEVEL_12_1,D3D_FEATURE_LEVEL_12_0
	};
	const char* featureLevelString[] = { "12.2","12.1" ,"12.0" };
	// 高い順に試す
	for (size_t i = 0; i < _countof(featureLevel); ++i) {
		// デバイス生成を検証
		HRESULT hr = D3D12CreateDevice(useAdapter.Get(), featureLevel[i], IID_PPV_ARGS(&device));
		if (SUCCEEDED(hr)) {
			// device生成完了通知
			Console("FeatureLevel : {}\n", featureLevelString[i]);
			break;
		}
	}
	// 失敗したら停止
	assert(device != nullptr);
}