#include "PrimitiveGeometryManager.h"

#include <mutex>

#include "Engine/Debug/Output.h"
#include "Engine/Resources/BackgroundLoader/BackgroundLoader.h"
#include "Engine/Utility/Tools/SmartPointer.h"
#include "PrimitiveGeometryResource.h"

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

std::mutex primitiveGeometryMutex;

std::shared_ptr<const PrimitiveGeometryResource> PrimitiveGeometryManager::GetPrimitiveGeometry(const std::string& name) {
	std::lock_guard<std::mutex> lock{ primitiveGeometryMutex };
	if (IsRegisteredNonlocking(name)) {
		return GetInstance().primitiveGeometryList.at(name);
	}
	else {
		//Console("Unloading . Name-\'{:}\'\n", name);
		return nullptr;
	}
}

bool PrimitiveGeometryManager::IsRegistered(const std::string& name) {
	std::lock_guard<std::mutex> lock{ primitiveGeometryMutex };
	return IsRegisteredNonlocking(name);
}

void PrimitiveGeometryManager::Transfer(const std::string& name, std::shared_ptr<PrimitiveGeometryResource> data) {
	std::lock_guard<std::mutex> lock{ primitiveGeometryMutex };
	Console("Transfer new line group. Name-\'{:}\', Address-\'{:}\'\n", name, (void*)data.get());
	GetInstance().primitiveGeometryList.emplace(name, data);
}

bool PrimitiveGeometryManager::IsRegisteredNonlocking(const std::string& name) {
	return GetInstance().primitiveGeometryList.contains(name);
}
