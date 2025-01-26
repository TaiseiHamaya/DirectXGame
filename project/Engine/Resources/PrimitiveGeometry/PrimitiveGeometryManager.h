#pragma once

#include "Engine/Utility/Template/SingletonInterface.h"

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

class PrimitiveGeometryResource;

class PrimitiveGeometryManager final : SingletonInterface<PrimitiveGeometryManager> {
	__SINGLETON_INTERFACE(PrimitiveGeometryManager)

public:
	/// <summary>
	/// ロードキューにファイルを追加
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	//static void RegisterLoadQue(const std::filesystem::path& filePath);

	/// <summary>
	/// 取得
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	static std::shared_ptr<const PrimitiveGeometryResource> GetPrimitiveGeometry(const std::string& name);

	/// <summary>
	/// 登録チェック
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	static bool IsRegistered(const std::string& name);

	/// <summary>
	/// nameとしてListに保存
	/// </summary>
	/// <param name="name">登録名</param>
	/// <param name="data">ロード済みデータ</param>
	static void Transfer(const std::string& name, std::shared_ptr<PrimitiveGeometryResource> data);

private:
	/// <summary>
	/// PrimitiveGeometryが登録されているか取得(mutexなし)
	/// </summary>
	/// <param name="name">確認するPrimitiveGeometry名</param>
	/// <returns>登録されていればtrue[bool]</returns>
	static bool IsRegisteredNonlocking(const std::string& name);

private:
	std::unordered_map<std::string, std::shared_ptr<PrimitiveGeometryResource>> primitiveGeometryList;
};
