#pragma once

#include <d3d12.h>
#include <dxcapi.h>
#include <string>
#include <wrl.h>

class ShaderCompiler final {
private:
	ShaderCompiler() = default;

public:
	~ShaderCompiler() = default;

private:
	ShaderCompiler(const ShaderCompiler&) = delete;
	ShaderCompiler& operator=(const ShaderCompiler&) = delete;

public:
	static void Initialize();
	static ShaderCompiler& GetInstance();

public:
	[[nodiscard]] Microsoft::WRL::ComPtr<IDxcBlob> compile_shader(const std::wstring& filePath, const wchar_t* profile);

private:
	void initialize();

private:
	Microsoft::WRL::ComPtr<IDxcUtils> dxcUtils;
	Microsoft::WRL::ComPtr<IDxcCompiler3> dxcCompiler;
	Microsoft::WRL::ComPtr<IDxcIncludeHandler> includeHandler;
};
