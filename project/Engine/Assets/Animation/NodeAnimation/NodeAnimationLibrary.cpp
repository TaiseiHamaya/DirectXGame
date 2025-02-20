#include "NodeAnimationLibrary.h"

#include <mutex>

#include "./NodeAnimationAsset.h"
#include "./NodeAnimationAssetBuilder.h"
#include "Engine/Assets/BackgroundLoader/BackgroundLoader.h"
#include "Engine/Debug/Output.h"
#include "Library/Utility/Tools/SmartPointer.h"

#ifdef _DEBUG
#include <ranges>
#include <imgui.h>
#endif // _DEBUG

std::mutex nodeAnimationMutex;

void NodeAnimationLibrary::RegisterLoadQue(const std::filesystem::path& filePath) {
	// ロード済みの場合は何もしない
	if (IsRegistered(filePath.filename().string())) {
		return;
	}
	// BackgroundLoaderにイベント送信
	BackgroundLoader::RegisterLoadQue(eps::CreateUnique<NodeAnimationAssetBuilder>(filePath));
}

std::shared_ptr<const NodeAnimationAsset> NodeAnimationLibrary::GetAnimation(const std::string& name) {
	std::lock_guard<std::mutex> lock{ nodeAnimationMutex };
	if (IsRegisteredNonlocking(name)) {
		return GetInstance().instanceList.at(name);
	}
	else {
		//Console("Unloading . Name-\'{:}\'\n", name);
		return nullptr;
	}
}

bool NodeAnimationLibrary::IsRegistered(const std::string& name) {
	std::lock_guard<std::mutex> lock{ nodeAnimationMutex };
	return IsRegisteredNonlocking(name);
}

void NodeAnimationLibrary::Transfer(const std::string& name, std::shared_ptr<NodeAnimationAsset> data) {
	std::lock_guard<std::mutex> lock{ nodeAnimationMutex };
	Console("Transfer new node animation. Name-\'{:}\', Address-\'{:}\'\n", name, (void*)data.get());
	GetInstance().instanceList.emplace(name, data);
}

#ifdef _DEBUG
bool NodeAnimationLibrary::AnimationListGui(std::string& current) {
	bool changed = false;

	std::lock_guard<std::mutex> lock{ nodeAnimationMutex };
	if (ImGui::BeginCombo("AnimationList", current.c_str())) {
		auto&& list = GetInstance().instanceList;
		for (const auto& name : list | std::views::keys) {
			bool is_selected = (current == name);
			if (ImGui::Selectable(name.c_str(), is_selected)) {
				current = name;
				changed = true;
			}
			if (is_selected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();

	}
	return changed;
}
#endif // _DEBUG

bool NodeAnimationLibrary::IsRegisteredNonlocking(const std::string& name) {
	return GetInstance().instanceList.contains(name);
}
