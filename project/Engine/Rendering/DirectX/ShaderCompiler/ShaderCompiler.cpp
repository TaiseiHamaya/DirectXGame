#include "ShaderCompiler.h"

#include <cassert>

#include "Engine/Debug/Output.h"

#pragma comment(lib, "dxcompiler.lib")

void ShaderCompiler::Initialize() {
	GetInstance().initialize();
}

ShaderCompiler& ShaderCompiler::GetInstance() noexcept {
	static ShaderCompiler instance{};
	return instance;
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

[[nodiscard]] Microsoft::WRL::ComPtr<IDxcBlob>  ShaderCompiler::compile_shader(const std::wstring& filePath, const std::wstring& profile) {
	HRESULT hr;
	Console(L"Start compile shader. Path-\'{}\', Profile-\'{}\'\n", filePath, profile); // 開始ログ
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
		L"/I", L"./EngineResources/HLSL/",
		L"-T", profile.c_str(),
#ifdef _DEBUG
		L"-Zi", L"-Qembed_debug",
		L"-Od",
#endif // _DEBUG
		L"-Zpr"
		//L"/enable_unbounded_descriptor_tables"
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
		auto msg = shaderError->GetStringPointer();
		Console("{}", msg); // CEだったら停止
		assert(false);
	}

	Microsoft::WRL::ComPtr<IDxcBlob>  shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(shaderBlob.GetAddressOf()), nullptr); // 成功したので書き込み
	assert(SUCCEEDED(hr));
	Console(L"Compile succeeded.\n"); // 成功ログ

	return shaderBlob;
}
