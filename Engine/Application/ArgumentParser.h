#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include <Library/Utility/Template/SingletonInterface.h>

/// <summary>
/// コマンドライン引数管理/解析クラス
/// </summary>
class ArgumentParser final : public SingletonInterface<ArgumentParser> {
	__CLASS_SINGLETON_INTERFACE(ArgumentParser)

public:
	/// <summary>
	/// コマンドライン引数の解析
	/// </summary>
	static void Parse();

	/// <summary>
	/// コマンドライン引数の検索
	/// </summary>
	/// <param name="arg"></param>
	/// <returns></returns>
	static bool Contains(const std::string& arg);

	/// <summary>
	/// インデックスに対応するコマンドライン引数を取得
	/// ※範囲外チェックなし
	/// </summary>
	/// <param name="index"></param>
	/// <returns></returns>
	static const std::string& ValueByIndex(u64 index);

	/// <summary>
	/// 引数の値と等しいコマンドライン引数のインデックスを探す
	/// </summary>
	/// <param name="value"></param>
	/// <returns>存在しない場合はnulloptを返す</returns>
	static std::optional<u64> ArgIndexByValue(const std::string& value);

	/// <summary>
	/// prefixで始まるposition番目の引数を探す
	/// 計算量: O(N)
	/// </summary>
	/// <param name="prefix"></param>
	/// <param name="position"></param>
	/// <returns>存在しない場合はnulloptを返す</returns>
	static std::optional<std::string> FindValueStartWith(const std::string& prefix, u64 position = 0) noexcept;

private:
	std::vector<std::string> arguments;
	std::unordered_map<std::string, u64> argumentsInverseIndex;
};
