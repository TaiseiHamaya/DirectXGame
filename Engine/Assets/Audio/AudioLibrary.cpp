#include "AudioLibrary.h"

#include <mutex>

#include <Library/Utility/Tools/SmartPointer.h>

#include "./AudioAsset.h"
#include "./AudioAssetBuilder.h"
#include "Engine/Application/Output.h"
#include "Engine/Assets/BackgroundLoader/BackgroundLoader.h"

std::mutex audioMutex;

AudioLibrary::AudioLibrary() noexcept = default;

AudioLibrary::~AudioLibrary() noexcept = default;

AudioLibrary& AudioLibrary::GetInstance() noexcept {
	static AudioLibrary instance;
	return instance;
}

void AudioLibrary::Initialize() {
	auto&& instance = GetInstance();
	// nullインスタンスの追加
	Transfer("NULL", nullptr);
}

void AudioLibrary::Finalize() {
	auto&& instance = GetInstance();
	instance.audioResources.clear();
}

void AudioLibrary::RegisterLoadQue(const std::filesystem::path& filePath) {
	if (IsRegistered(filePath.filename().string())) {
		return;
	}
	// BackgroundLoaderにイベント送信
	BackgroundLoader::RegisterLoadQue(
		eps::CreateUnique<AudioAssetBuilder>(filePath)
	);
}

const std::unique_ptr<AudioAsset>& AudioLibrary::GetAudio(const std::string& audioName) {
	std::lock_guard<std::mutex> lock{ audioMutex };
	// 見つかったらそのデータのweak_ptrを返す
	if (IsRegisteredNonlocking(audioName)) {
		return GetInstance().audioResources.at(audioName);
	}
	else {
		Warning("Audio Name-\'{:}\' is not loading.", audioName);
		return GetInstance().audioResources.at("NULL");
	}
}

bool AudioLibrary::IsRegistered(const std::string& audioName) noexcept(false) {
	std::lock_guard<std::mutex> lock{ audioMutex };
	return IsRegisteredNonlocking(audioName);
}

void AudioLibrary::UnloadAudio(const std::string& audioName) {
	std::lock_guard<std::mutex> lock{ audioMutex };
	if (IsRegisteredNonlocking(audioName)) {
		GetInstance().audioResources.erase(audioName);
	}
}

void AudioLibrary::Transfer(const std::string& name, std::unique_ptr<AudioAsset>&& data) {
	std::lock_guard<std::mutex> lock{ audioMutex };
	if (IsRegisteredNonlocking(name)) {
		Warning("Transferring registered Audio. Name-\'{:}\', Address-\'{:016}\'", name, (void*)data.get());
		return;
	}
	Information("Transfer new Audio. Name-\'{:}\', Address-\'{:016}\'", name, (void*)data.get());
	GetInstance().audioResources.emplace(name, std::move(data));

}

bool AudioLibrary::IsRegisteredNonlocking(const std::string& audioName) noexcept(false) {
	return GetInstance().audioResources.contains(audioName);
}
