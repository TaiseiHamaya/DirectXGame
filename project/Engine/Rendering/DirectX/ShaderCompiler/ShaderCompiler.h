#pragma once

#include <string>

#include <d3d12.h>
#include <dxcapi.h>
#include <wrl.h>

class ShaderCompiler final {
private:
	ShaderCompiler() noexcept = default;

public:
	~ShaderCompiler() noexcept = default;

private:
	ShaderCompiler(const ShaderCompiler&) = delete;
	ShaderCompiler& operator=(const ShaderCompiler&) = delete;

public:
	static void Initialize();
	static ShaderCompiler& GetInstance() noexcept;

public:
	[[nodiscard]] Microsoft::WRL::ComPtr<IDxcBlob> compile_shader(const std::wstring& filePath, const std::wstring& profile);

private:
	void initialize();

private:
	Microsoft::WRL::ComPtr<IDxcUtils> dxcUtils;
	Microsoft::WRL::ComPtr<IDxcCompiler3> dxcCompiler;
	Microsoft::WRL::ComPtr<IDxcIncludeHandler> includeHandler;
};
