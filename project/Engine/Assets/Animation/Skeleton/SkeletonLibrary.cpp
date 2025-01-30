#include "SkeletonLibrary.h"

#include <mutex>

#include "./SkeletonAsset.h"
#include "./SkeletonAssetBuilder.h"
#include "Engine/Assets/BackgroundLoader/BackgroundLoader.h"
#include "Engine/Debug/Output.h"

#include <Library/Utility/Tools/SmartPointer.h>

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

std::mutex skeletonMutex;

void SkeletonLibrary::RegisterLoadQue(const std::filesystem::path& filePath) {
	// ロード済みの場合は何もしない
	if (IsRegistered(filePath.filename().string())) {
		return;
	}
	// BackgroundLoaderにイベント送信
	BackgroundLoader::RegisterLoadQue(eps::CreateUnique<SkeletonAssetBuilder>(filePath));
}

std::shared_ptr<const SkeletonAsset> SkeletonLibrary::GetSkeleton(const std::string& name) {
	std::lock_guard<std::mutex> lock{ skeletonMutex };
	if (IsRegisteredNonlocking(name)) {
		return GetInstance().instanceList.at(name);
	}
	else {
		Console("Warning : Unloading skeleton. Name-\'{:}\'\n", name);
		return nullptr;
	}
}

bool SkeletonLibrary::IsRegistered(const std::string& name) {
	std::lock_guard<std::mutex> lock{ skeletonMutex };
	return IsRegisteredNonlocking(name);

}

void SkeletonLibrary::Transfer(const std::string& name, std::shared_ptr<SkeletonAsset>& data) {
	std::lock_guard<std::mutex> lock{ skeletonMutex };
	Console("Transfer new skeleton. Name-\'{:}\', Address-\'{:}\'\n", name, (void*)data.get());
	GetInstance().instanceList.emplace(name, data);
}

//bool SkeletonLibrary::SkeletonListGui(std::string& current) {
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

bool SkeletonLibrary::IsRegisteredNonlocking(const std::string& name) {
	return GetInstance().instanceList.contains(name);
}
