#pragma once

#include <any>
#include <string>
#include <unordered_map>

#include <Library/Utility/Template/SingletonInterface.h>

class RuntimeStorage : public SingletonInterface<RuntimeStorage> {
	__CLASS_SINGLETON_INTERFACE(RuntimeStorage)

public:
	struct ValueGroup {
		std::unordered_map<std::string, std::any> values;
	};

public:
	static ValueGroup& GetValue(const std::string& name);

private:
	std::unordered_map<std::string, ValueGroup> runtimeValues;
};

