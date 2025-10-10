#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include <Library/Utility/Template/SingletonInterface.h>

class ArgumentParser final : public SingletonInterface<ArgumentParser> {
	__CLASS_SINGLETON_INTERFACE(ArgumentParser)

public:
	static void Parse();

	static bool Contains(const std::string& arg);

	static const std::string& ValueByIndex(u64 index);

	static const std::string& ValueOr(const std::string& arg, const std::string& defaultValue = "");

	static std::optional<u64> ArgIndexByValue(const std::string& value);

private:
	std::vector<std::string> arguments;
	std::unordered_map<std::string, u64> argumentsInverseIndex;
};
