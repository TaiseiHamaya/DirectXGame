#pragma once

#include "Library/Utility/Template/SingletonInterface.h"

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

class SkeletonAsset;

class SkeletonLibrary final : SingletonInterface<SkeletonLibrary> {
	__CLASS_SINGLETON_INTERFACE(SkeletonLibrary)

public:
	/// <summary>
	/// ロードキューにファイルを追加
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	static void RegisterLoadQue(const std::filesystem::path& filePath);

	/// <summary>
	/// Skeletonの取得
	/// </summary>
	/// <param name="name">Skeleton名</param>
	/// <returns>SkeletonResourceクラスのshared_ptr</returns>
	static std::shared_ptr<const SkeletonAsset> GetSkeleton(const std::string& name);

	/// <summary>
	/// Skeletonが登録されているか取得
	/// </summary>
	/// <param name="name">確認するSkeleton名</param>
	/// <returns>登録されていればtrue[bool]</returns>
	static bool IsRegistered(const std::string& name);

	/// <summary>
	/// nameとしてListに保存
	/// </summary>
	/// <param name="name">登録名</param>
	/// <param name="data">ロード済みデータ</param>
	static void Transfer(const std::string& name, std::shared_ptr<SkeletonAsset>& data);

#ifdef _DEBUG
	/// <summary>
	/// Skeleton一覧をComboBoxで表示するImGui(Debugビルドのみ)
	/// </summary>
	/// <param name="current">現在選択中のSkeleton名</param>
	/// <returns>currentが変更されたかどうか</returns>
	//static bool SkeletonListGui(std::string& current);
#endif // _DEBUG

private:
	/// <summary>
	/// Skeletonが登録されているか取得(mutexなし)
	/// </summary>
	/// <param name="name">確認するSkeleton名</param>
	/// <returns>登録されていればtrue[bool]</returns>
	static bool IsRegisteredNonlocking(const std::string& name);

private:
	std::unordered_map<std::string, std::shared_ptr<SkeletonAsset>> instanceList;
};
