#include "SkeletonManager.h"

#include <mutex>

#include "Engine/Debug/Output.h"
#include "Engine/Resources/BackgroundLoader/BackgroundLoader.h"
#include "Engine/Utility/Tools/SmartPointer.h"
#include "SkeletonResource.h"
#include "SkeletonResourceBuilder.h"

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

std::mutex skeletonMutex;

void SkeletonManager::RegisterLoadQue(const std::filesystem::path& filePath) {
	// ロード済みの場合は何もしない
	if (IsRegistered(filePath.filename().string())) {
		return;
	}
	// BackgroundLoaderにイベント送信
	BackgroundLoader::RegisterLoadQue(eps::CreateUnique<SkeletonResourceBuilder>(filePath));
}

std::shared_ptr<const SkeletonResource> SkeletonManager::GetSkeleton(const std::string& name) {
	std::lock_guard<std::mutex> lock{ skeletonMutex };
	if (IsRegisteredNonlocking(name)) {
		return GetInstance().instanceList.at(name);
	}
	else {
		Console("Warning : Unloading skeleton. Name-\'{:}\'\n", name);
		return nullptr;
	}
}

bool SkeletonManager::IsRegistered(const std::string& name) {
	std::lock_guard<std::mutex> lock{ skeletonMutex };
	return IsRegisteredNonlocking(name);

}

void SkeletonManager::Transfer(const std::string& name, std::shared_ptr<SkeletonResource>& data) {
	std::lock_guard<std::mutex> lock{ skeletonMutex };
	Console("Transfer new skeleton. Name-\'{:}\', Address-\'{:}\'\n", name, (void*)data.get());
	GetInstance().instanceList.emplace(name, data);
}

//bool SkeletonManager::SkeletonListGui(std::string& current) {
//	bool changed = false;
//
//	std::lock_guard<std::mutex> lock{ skeletonMutex };
//	if (ImGui::BeginCombo("SkeletonList", current.c_str())) {
//		auto&& list = GetInstance().instanceList;
//		for (auto itr = list.begin(); itr != list.end(); ++itr) {
//			bool is_selected = (current == itr->first);
//			if (ImGui::Selectable(itr->first.c_str(), is_selected)) {
//				current = itr->first;
//				changed = true;
//			}
//			if (is_selected) {
//				ImGui::SetItemDefaultFocus();
//			}
//		}
//		ImGui::EndCombo();
//
//	}
//	return changed;
//}

bool SkeletonManager::IsRegisteredNonlocking(const std::string& name) {
	return GetInstance().instanceList.contains(name);
}
