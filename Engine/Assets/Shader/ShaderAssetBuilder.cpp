#include "ShaderAssetBuilder.h"

#include <Library/Utility/Tools/SmartPointer.h>

#include "./ShaderAsset.h"
#include "./ShaderLibrary.h"
#include "Engine/Application/Output.h"
#include "Engine/GraphicsAPI/DirectX/DxCompiler/DxcManager.h"
#include "Engine/GraphicsAPI/DirectX/DxSystemValues.h"

constexpr wstring_literal extension[] = {
	L"VS.hlsl",
	L"PS.hlsl",
	L"CS.hlsl",
	L"MS.hlsl",
};

const std::wstring profiles[] = {
	std::format(L"vs_{}_{}", DxSystemValues::SHADER_VERSION.first, DxSystemValues::SHADER_VERSION.second),
	std::format(L"ps_{}_{}", DxSystemValues::SHADER_VERSION.first, DxSystemValues::SHADER_VERSION.second),
	std::format(L"cs_{}_{}", DxSystemValues::SHADER_VERSION.first, DxSystemValues::SHADER_VERSION.second),
	std::format(L"ms_{}_{}", DxSystemValues::SHADER_VERSION.first, DxSystemValues::SHADER_VERSION.second),
};

ShaderAssetBuilder::ShaderAssetBuilder(const std::filesystem::path& filePath_) {
	filePath = filePath_;
}

void ShaderAssetBuilder::preprocess() {
	asset = eps::CreateShared<ShaderAsset>();
}

bool ShaderAssetBuilder::run() {
	HRESULT hr;
	Information(L"Start compile shader. Path-\'{}\'", filePath.native()); // 開始ログ

	std::wstring_view profile;
	for (u32 i = 0; i < 4; ++i) {
		if (filePath.native().ends_with(extension[i])) {
			profile = profiles[i];
			break;
		}
	}
	ErrorIf(profile.empty(), L"Failed to find shader profile in extension. File-\'{}\'", filePath.native());

	Microsoft::WRL::ComPtr<IDxcBlobEncoding> shaderSource = nullptr;
	hr = DxcManager::GetUtils().LoadFile(filePath.c_str(), nullptr, shaderSource.GetAddressOf()); // ロード
	// ファイル読み込みエラー
	ErrorIf(FAILED(hr), L"Failed to load the shader file. File-\'{}\'", filePath.native());

	DxcBuffer shaderSourceBuffer{}; // ロードしたデータを読み込む
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;

	LPCWSTR arguments[] = { // 追加パラメータ
		filePath.c_str(),
		L"-E", L"main", // エントリーポイント
		L"-I", L"./DirectXGame/EngineResources/HLSL/", // 追加のインクルードディレクトリ
		L"-T", profile.data(), // シェーダーの種類
#ifdef DEBUG_FEATURES_ENABLE
		L"-Zi", L"-Qembed_debug", // デバッグ機能
		L"-Od", // 最適化なし
#else
		L"-O2", // 最適化Lv2
#endif // _DEBUG
		L"-Zpr",
		L"-WX"
	};
	Microsoft::WRL::ComPtr<IDxcResult> shaderResult = nullptr;
	hr = DxcManager::GetCompiler().Compile( // ここでコンパイル
		&shaderSourceBuffer,
		arguments,
		_countof(arguments),
		&DxcManager::GetIncludeHandler(),
		IID_PPV_ARGS(shaderResult.GetAddressOf())
	);
	// コンパイルエラー
	ErrorIf(FAILED(hr), L"Failed compile shader. File-\'{}\'", filePath.native());

	// シェーダーのコンパイル結果をチェック
	Microsoft::WRL::ComPtr<IDxcBlobUtf8> shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(shaderError.GetAddressOf()), nullptr); // エラーを取得
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		auto msg = shaderError->GetStringPointer();
		Error("HLSL shader compile error. Message-\'{}\'", msg); // CEだったら停止
	}

	// 書き込み処理
	Microsoft::WRL::ComPtr<IDxcBlob>  shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(shaderBlob.GetAddressOf()), nullptr); // 成功したので書き込み
	ErrorIf(FAILED(hr), L"Shader compilation succeeded, but writing failed. File-\'{}\'", filePath.native());
	Information(L"Compile succeeded."); // 成功ログ

	asset->initialize(shaderBlob);

	return true;
}

void ShaderAssetBuilder::postprocess() {
	// Do nothing
}

void ShaderAssetBuilder::transfer() {
	ShaderLibrary::Transfer(filePath.filename().string(), asset);
}
