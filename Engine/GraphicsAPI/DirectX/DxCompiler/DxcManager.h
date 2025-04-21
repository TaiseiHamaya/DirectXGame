#pragma once

#include <filesystem>

#include <d3d12.h>
#include <dxcapi.h>
#include <wrl.h>

#include <Library/Utility/Template/SingletonInterface.h>

class DxcManager final : public SingletonInterface<DxcManager> {
	__CLASS_SINGLETON_INTERFACE(DxcManager)

public:
	static void Initialize();

public:
	static IDxcUtils& GetUtils() { return *GetInstance().dxcUtils.Get(); }
	static IDxcCompiler3& GetCompiler() { return *GetInstance().dxcCompiler.Get(); }
	static IDxcIncludeHandler& GetIncludeHandler() { return *GetInstance().includeHandler.Get(); }

private:
	void initialize();

private:
	Microsoft::WRL::ComPtr<IDxcUtils> dxcUtils;
	Microsoft::WRL::ComPtr<IDxcCompiler3> dxcCompiler;
	Microsoft::WRL::ComPtr<IDxcIncludeHandler> includeHandler;
};
