#include "ShaderCompiler.h"

#include "Engine/Utility/Utility.h"
#include <cassert>
#include <format>
#include <memory>
#pragma comment(lib, "dxcompiler.lib")

void ShaderCompiler::Initialize() {
	GetInstance().initialize();
}

ShaderCompiler& ShaderCompiler::GetInstance() noexcept {
	static std::unique_ptr<ShaderCompiler> instance{ new ShaderCompiler };
	return *instance;
}

void ShaderCompiler::initialize() {
	HRESULT hr;
	// ----------コンパイラの生成----------
	hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(dxcUtils.GetAddressOf()));
	assert(SUCCEEDED(hr));
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(dxcCompiler.GetAddressOf()));
	assert(SUCCEEDED(hr));
	hr = dxcUtils->CreateDefaultIncludeHandler(includeHandler.GetAddressOf());
	assert(SUCCEEDED(hr));
}

[[nodiscard]] Microsoft::WRL::ComPtr<IDxcBlob>  ShaderCompiler::compile_shader(const std::wstring& filePath, const wchar_t* profile) {
	HRESULT hr;
	Log(std::format(L"[ShaderCompiler] Start compile shader. Path-\'{}\', Profile-\'{}\'\n", filePath, profile)); // 開始ログ
	Microsoft::WRL::ComPtr<IDxcBlobEncoding> shaderSource = nullptr;
	hr = dxcUtils->LoadFile(filePath.c_str(), nullptr, shaderSource.GetAddressOf()); // ロード
	assert(SUCCEEDED(hr)); // ファイル読み込みエラー

	DxcBuffer shaderSourceBuffer; // ロードしたデータを読み込む
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;

	LPCWSTR arguments[] = { // 追加パラメータ
		filePath.c_str(),
		L"-E", L"main",
		L"-T", profile,
		L"-Zi", L"-Qembed_debug",
		L"-Od",
		L"-Zpr"
	};
	Microsoft::WRL::ComPtr<IDxcResult> shaderResult = nullptr;
	hr = dxcCompiler->Compile( // ここでコンパイル
		&shaderSourceBuffer,
		arguments,
		_countof(arguments),
		includeHandler.Get(),
		IID_PPV_ARGS(shaderResult.GetAddressOf())
	);
	assert(SUCCEEDED(hr));

	Microsoft::WRL::ComPtr<IDxcBlobUtf8> shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(shaderError.GetAddressOf()), nullptr); // エラーを取得
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		Log(shaderError->GetStringPointer()); // CEだったら停止
		assert(false);
	}

	Microsoft::WRL::ComPtr<IDxcBlob>  shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(shaderBlob.GetAddressOf()), nullptr); // 成功したので書き込み
	assert(SUCCEEDED(hr));
	Log(std::format(L"[ShaderCompiler] Compile succeeded. Path-\'{}\', Profile-\'{}\'\n", filePath, profile)); // 成功ログ

	return shaderBlob;
}
