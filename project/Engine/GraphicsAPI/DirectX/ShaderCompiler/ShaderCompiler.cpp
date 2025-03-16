#include "ShaderCompiler.h"

#include "Engine/Application/Output.h"

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
	CriticalIf(FAILED(hr), "Failed to initialize Shader compiler.");
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(dxcCompiler.GetAddressOf()));
	CriticalIf(FAILED(hr), "Failed to initialize Shader compiler.");
	hr = dxcUtils->CreateDefaultIncludeHandler(includeHandler.GetAddressOf());
	CriticalIf(FAILED(hr), "Failed to initialize Shader compiler.");
}

[[nodiscard]] Microsoft::WRL::ComPtr<IDxcBlob>  ShaderCompiler::compile_shader(const std::wstring& filePath, const std::wstring& profile) {
	HRESULT hr;
	Infomation(L"Start compile shader. Path-\'{}\', Profile-\'{}\'", filePath, profile); // 開始ログ
	Microsoft::WRL::ComPtr<IDxcBlobEncoding> shaderSource = nullptr;
	hr = dxcUtils->LoadFile(filePath.c_str(), nullptr, shaderSource.GetAddressOf()); // ロード
	// ファイル読み込みエラー
	ErrorIf(FAILED(hr), "Failed to load the shader file.");

	DxcBuffer shaderSourceBuffer; // ロードしたデータを読み込む
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;

	LPCWSTR arguments[] = { // 追加パラメータ
		filePath.c_str(),
		L"-E", L"main",
		L"-I", L"./EngineResources/HLSL/",
		L"-T", profile.c_str(),
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
	hr = dxcCompiler->Compile( // ここでコンパイル
		&shaderSourceBuffer,
		arguments,
		_countof(arguments),
		includeHandler.Get(),
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

	return shaderBlob;
}
