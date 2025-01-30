#include "PrimitiveGeometryLibrary.h"

#include <mutex>

#include "Library/Utility/Tools/SmartPointer.h"

#include "./PrimitiveGeometryAsset.h"
#include "Engine/Assets/BackgroundLoader/BackgroundLoader.h"
#include "Engine/Debug/Output.h"

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

std::mutex primitiveGeometryMutex;

std::shared_ptr<const PrimitiveGeometryAsset> PrimitiveGeometryLibrary::GetPrimitiveGeometry(const std::string& name) {
	std::lock_guard<std::mutex> lock{ primitiveGeometryMutex };
	if (IsRegisteredNonlocking(name)) {
		return GetInstance().primitiveGeometryList.at(name);
	}
	else {
		//Console("Unloading . Name-\'{:}\'\n", name);
		return nullptr;
	}
}

bool PrimitiveGeometryLibrary::IsRegistered(const std::string& name) {
	std::lock_guard<std::mutex> lock{ primitiveGeometryMutex };
	return IsRegisteredNonlocking(name);
}

void PrimitiveGeometryLibrary::Transfer(const std::string& name, std::shared_ptr<PrimitiveGeometryAsset> data) {
	std::lock_guard<std::mutex> lock{ primitiveGeometryMutex };
	Console("Transfer new line group. Name-\'{:}\', Address-\'{:}\'\n", name, (void*)data.get());
	GetInstance().primitiveGeometryList.emplace(name, data);
}

bool PrimitiveGeometryLibrary::IsRegisteredNonlocking(const std::string& name) {
	return GetInstance().primitiveGeometryList.contains(name);
}
