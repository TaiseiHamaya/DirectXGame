#include "RuntimeStorage.h"

#include "Engine/Application/Output.h"

RuntimeStorage::ValueGroup& RuntimeStorage::GetValue(const std::string& name) {
	auto& instance = GetInstance();
	if (!instance.runtimeValues.contains(name)) {
		Information("Create value group. Name-\'{}\'", name);
	}
	return instance.runtimeValues[name];
}
