#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

#include <Library/Utility/Template/SingletonInterface.h>

class ShaderAsset;

class ShaderLibrary final : public SingletonInterface<ShaderLibrary> {
	__CLASS_SINGLETON_INTERFACE(ShaderLibrary)

public:
	static void RegisterLoadQue(const std::filesystem::path& filePath);

	static std::shared_ptr<const ShaderAsset> GetShader(const std::string& name);

	static bool IsRegistered(const std::string& meshName);

	static void Transfer(const std::string& name, std::shared_ptr<ShaderAsset>& data);

//#ifdef DEBUG_FEATURES_ENABLE
//	/// <summary>
//	/// メッシュ一覧をComboBoxで表示するImGui(Debugビルドのみ)
//	/// </summary>
//	/// <param name="current">現在選択中のメッシュ名</param>
//	/// <returns>currentが変更されたかどうか</returns>
//	static bool MeshListGui(std::string& current);
//#endif // _DEBUG

private:
	static bool IsRegisteredNonlocking(const std::string& name);

private:
	std::unordered_map<std::string, std::shared_ptr<ShaderAsset>> assets;
};
