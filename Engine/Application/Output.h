#pragma once

#include <format>
#include <source_location>
#include <string>

enum class LogType : u8 {
	//Trace,
	Information,
	Warning,
	Error,
	Critical,
	// Assert,
	// AssertDebug,
	// 
};

void InitializeLog();

void SyncErrorWindow();

void LogOutputA(const std::source_location& sourceLocation, LogType type, const std::string& message);

void LogOutputW(const std::source_location& sourceLocation, LogType type, const std::wstring& message);

template<typename ...Args>
inline void ConsoleFormattingSL(const std::source_location& sourceLocation, LogType type, std::format_string<Args...> msg, Args && ...args) {
	std::string message = std::format(msg, std::forward<Args>(args)...);
	LogOutputA(sourceLocation, type, message);
}

template<typename ...Args>
inline void ConsoleFormattingSL(const std::source_location& sourceLocation, LogType type, std::wformat_string<Args...> msg, Args && ...args) {
	std::wstring message = std::format(msg, std::forward<Args>(args)...);
	LogOutputW(sourceLocation, type, message);
}

#define Information(msg, ...) ConsoleFormattingSL(std::source_location::current(), LogType::Information, msg, __VA_ARGS__)
#define Warning(msg, ...) ConsoleFormattingSL(std::source_location::current(), LogType::Warning, msg, __VA_ARGS__)
#define Error(msg, ...) ConsoleFormattingSL(std::source_location::current(), LogType::Error, msg, __VA_ARGS__)
#define Critical(msg, ...) ConsoleFormattingSL(std::source_location::current(), LogType::Critical, msg, __VA_ARGS__)

#define WarningIf(conditional, msg, ...) if((conditional)) Warning(msg, __VA_ARGS__)
#define ErrorIf(conditional, msg, ...) if((conditional)) Error(msg, __VA_ARGS__)
#define CriticalIf(conditional, msg, ...) if((conditional)) Critical(msg, __VA_ARGS__)

/*

# エラー区分メモ

## information
通常の出力

## Warning
問題が発生したが、継続可能なエラー
例 : 存在しないAssetの使用、無効な値の代入

## Error
復帰が難しいが、一時的な継続が可能であるエラー
例 : 初期化ミス、ロード失敗、シェーダーコンパイルエラー

## Critical
復帰不可能な場合。クラッシュするようなもの。
例 : システム使用のクラスの初期化失敗、メモリアロケーションの失敗、メモリ破壊

*/