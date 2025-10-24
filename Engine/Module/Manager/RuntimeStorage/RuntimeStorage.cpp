#include "RuntimeStorage.h"

#include "Engine/Application/Logger.h"

RuntimeStorage::ValueGroup& RuntimeStorage::GetValue(const std::string& name) {
	auto& instance = GetInstance();
	if (!instance.runtimeValues.contains(name)) {
		szgInformation("Create value group. Name-\'{}\'", name);
	}
	return instance.runtimeValues[name];
}
