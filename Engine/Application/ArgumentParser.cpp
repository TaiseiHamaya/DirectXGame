#include "ArgumentParser.h"

#include <sstream>

#include <windows.h>

#include <Library/Utility/Tools/ConvertString.h>

void ArgumentParser::Parse() {
	auto& instance = GetInstance();
	instance.arguments.clear();
	instance.argumentsInverseIndex.clear();

	wchar_t* commandLine = GetCommandLineW();

	// スペース区切りで格納
	std::wstringstream stream;
	stream << commandLine;
	i32 index = 0;
	while (!stream.eof()) {
		std::wstring argW;
		stream >> argW;
		if (argW.length() == 0) {
			continue;
		}
		std::string argA = ConvertString(argW);
		instance.arguments.emplace_back(argA);
		instance.argumentsInverseIndex[argA] = index;
		++index;
	}
}

bool ArgumentParser::Contains(const std::string& arg) {
	return GetInstance().argumentsInverseIndex.contains(arg);
}

const std::string& ArgumentParser::ValueByIndex(u64 index) {
	return GetInstance().arguments[index];
}

const std::string& ArgumentParser::ValueOr(const std::string& arg, const std::string& defaultValue) {
	auto& instance = GetInstance();
	if (!instance.argumentsInverseIndex.contains(arg)) {
		return defaultValue;
	}
	u64 index = instance.argumentsInverseIndex[arg];
	return instance.arguments[index];
}

std::optional<u64> ArgumentParser::ArgIndexByValue(const std::string& value) {
	auto& instance = GetInstance();
	if (!instance.argumentsInverseIndex.contains(value)) {
		return std::nullopt;
	}
	return instance.argumentsInverseIndex[value];
}
