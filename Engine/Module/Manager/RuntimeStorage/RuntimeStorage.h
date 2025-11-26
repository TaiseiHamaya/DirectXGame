#pragma once

#include <any>
#include <string>
#include <unordered_map>
#include <optional>

#include <Library/Utility/Template/SingletonInterface.h>
#include <Library/Utility/Template/Reference.h>

class RuntimeStorage : public SingletonInterface<RuntimeStorage> {
	__CLASS_SINGLETON_INTERFACE(RuntimeStorage)

public:
	using ValueGroup = std::unordered_map<std::string, std::any>;

public:
	static ValueGroup& GetValueList(const std::string& name);

	static Reference<const std::any> GetValueImm(const std::string& groupName, const std::string& valueName);
	static Reference<std::any> GetValueMut(const std::string& groupName, const std::string& valueName);

private:
	std::unordered_map<std::string, ValueGroup> runtimeValues;
};

