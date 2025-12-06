#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

#include <Library/Utility/Template/SingletonInterface.h>

namespace szg {

class ShaderAsset;

/// <summary>
/// シェーダーアセット保持クラス(スレッドセーフ)
/// </summary>
class ShaderLibrary final : public SingletonInterface<ShaderLibrary> {
	SZG_CLASS_SINGLETON(ShaderLibrary)

public:
	/// <summary>
	/// ロードキューにhlslファイルを追加
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	static void RegisterLoadQue(const std::filesystem::path& filePath);

	/// <summary>
	/// シェーダーの取得
	/// </summary>
	/// <param name="name">シェーダー名</param>
	/// <returns></returns>
	static std::shared_ptr<const ShaderAsset> GetShader(const std::string& name);

	/// <summary>
	/// シェーダーが登録されているか取得
	/// </summary>
	/// <param name="name">シェーダー名</param>
	/// <returns></returns>
	static bool IsRegistered(const std::string& name);

	/// <summary>
	/// 転送し登録
	/// </summary>
	/// <param name="name">シェーダー名</param>
	/// <param name="data">ロード済みアセットデータ</param>
	static void Transfer(const std::string& name, std::shared_ptr<ShaderAsset>& data);

private:
	static bool IsRegisteredNonlocking(const std::string& name);

private:
	std::unordered_map<std::string, std::shared_ptr<ShaderAsset>> assets;
};

}; // szg
