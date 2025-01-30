#pragma once

#include "Library/Utility/Template/SingletonInterface.h"

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

class NodeAnimationAsset;

class NodeAnimationLibrary final : SingletonInterface<NodeAnimationLibrary> {
	__SINGLETON_INTERFACE(NodeAnimationLibrary)

public:
	/// <summary>
	/// ロードキューにファイルを追加
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	static void RegisterLoadQue(const std::filesystem::path& filePath);

	/// <summary>
	/// Animationの取得
	/// </summary>
	/// <param name="name">Animation名</param>
	/// <returns>NodeAnimationResourceクラスのshared_ptr</returns>
	static std::shared_ptr<const NodeAnimationAsset> GetAnimation(const std::string& name);

	/// <summary>
	/// Animationが登録されているか取得
	/// </summary>
	/// <param name="name">確認するAnimation名</param>
	/// <returns>登録されていればtrue[bool]</returns>
	static bool IsRegistered(const std::string& name);

	/// <summary>
	/// nameとしてListに保存
	/// </summary>
	/// <param name="name">登録名</param>
	/// <param name="data">ロード済みデータ</param>
	static void Transfer(const std::string& name, std::shared_ptr<NodeAnimationAsset>& data);

#ifdef _DEBUG
	/// <summary>
	/// Animation一覧をComboBoxで表示するImGui(Debugビルドのみ)
	/// </summary>
	/// <param name="current">現在選択中のAnimation名</param>
	/// <returns>currentが変更されたかどうか</returns>
	//static bool AnimationListGui(std::string& current);
#endif // _DEBUG

private:
	/// <summary>
	/// Animationが登録されているか取得(mutexなし)
	/// </summary>
	/// <param name="name">確認するAnimation名</param>
	/// <returns>登録されていればtrue[bool]</returns>
	static bool IsRegisteredNonlocking(const std::string& name);

private:
	std::unordered_map<std::string, std::shared_ptr<NodeAnimationAsset>> instanceList;
};

