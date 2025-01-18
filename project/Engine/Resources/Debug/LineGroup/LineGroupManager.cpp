#include "LineGroupManager.h"

#ifdef _DEBUG

#include <mutex>

#include "Engine/Debug/Output.h"
#include "Engine/Resources/BackgroundLoader/BackgroundLoader.h"
#include "Engine/Utility/Tools/SmartPointer.h"
#include "LineGroupResource.h"

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

std::mutex lineGroupMutex;

std::shared_ptr<const LineGroupResource> LineGroupManager::GetLineGroup(const std::string& name) {
	std::lock_guard<std::mutex> lock{ lineGroupMutex };
	if (IsRegisteredNonlocking(name)) {
		return GetInstance().lineGroupList.at(name);
	}
	else {
		//Console("Unloading . Name-\'{:}\'\n", name);
		return nullptr;
	}
}

bool LineGroupManager::IsRegistered(const std::string& name) {
	std::lock_guard<std::mutex> lock{ lineGroupMutex };
	return IsRegisteredNonlocking(name);
}

void LineGroupManager::Transfer(const std::string& name, std::shared_ptr<LineGroupResource> data) {
	std::lock_guard<std::mutex> lock{ lineGroupMutex };
	Console("Transfer new line group. Name-\'{:}\', Address-\'{:}\'\n", name, (void*)data.get());
	GetInstance().lineGroupList.emplace(name, data);
}

bool LineGroupManager::IsRegisteredNonlocking(const std::string& name) {
	return GetInstance().lineGroupList.contains(name);
}

#endif // _DEBUG
