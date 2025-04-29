#include "ShaderAsset.h"

#include "Engine/Application/Output.h"
#include "Engine/GraphicsAPI/DirectX/DxSystemValues.h"
#include "Engine/GraphicsAPI/DirectX/DxCompiler/DxcManager.h"

using namespace std::literals::string_literals;

constexpr wstring_literal filetags[] = {
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

bool ShaderAsset::load(const std::filesystem::path& filePath) {
	HRESULT hr;
	Infomation(L"Start compile shader. Path-\'{}\'", filePath.native()); // 開始ログ

	std::wstring_view profile;
	for (u32 i = 0; i < 4; ++i) {
		if (filePath.native().ends_with(filetags[i])) {
			profile = profiles[i];
			break;
		}
	}
	ErrorIf(profile.empty(), L"Failed to find shader profile. File-\'{}\'", filePath.native());
	
	Microsoft::WRL::ComPtr<IDxcBlobEncoding> shaderSource = nullptr;
	hr = DxcManager::GetUtils().LoadFile(filePath.c_str(), nullptr, shaderSource.GetAddressOf()); // ロード
	// ファイル読み込みエラー
	ErrorIf(FAILED(hr), "Failed to load the shader file.");

	DxcBuffer shaderSourceBuffer; // ロードしたデータを読み込む
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;

	LPCWSTR arguments[] = { // 追加パラメータ
		filePath.c_str(),
		L"-E", L"main",
		L"-I", L"./DirectXGame/EngineResources/HLSL/",
		L"-T", profile.data(),
#ifdef DEBUG_FEATURES_ENABLE
		L"-Zi", L"-Qembed_debug",
		L"-Od",
#else
		L"-O2",
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
	ErrorIf(FAILED(hr), "Failed compile shader.");

	Microsoft::WRL::ComPtr<IDxcBlobUtf8> shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(shaderError.GetAddressOf()), nullptr); // エラーを取得
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		auto msg = shaderError->GetStringPointer();
		Error("HLSL shader compile error. Message-\'{}\'", msg); // CEだったら停止
	}

	Microsoft::WRL::ComPtr<IDxcBlob>  shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(shaderBlob.GetAddressOf()), nullptr); // 成功したので書き込み
	ErrorIf(FAILED(hr), "Shader compilation succeeded, but writing failed.");
	Infomation(L"Compile succeeded."); // 成功ログ

	blob = shaderBlob;

	return true;
}

D3D12_SHADER_BYTECODE ShaderAsset::blob_bytecode() const noexcept {
	return { blob->GetBufferPointer(), blob->GetBufferSize() };
}

DxcBuffer ShaderAsset::blob_buffer() const noexcept {
	return { blob->GetBufferPointer(), blob->GetBufferSize(), 0 };
}
