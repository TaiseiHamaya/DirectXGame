#include "AudioManager.h"

#pragma comment(lib, "xaudio2.lib")

#include <fstream>
#include <mutex>
#include <format>

#include "Engine/Utility/BackgroundLoader/BackgroundLoader.h"
#include "Engine/Game/Audio/AudioResource.h"
#include <Engine/Utility/Utility.h>

std::mutex audioMutex;

AudioManager::AudioManager() noexcept = default;

AudioManager::~AudioManager() noexcept = default;

AudioManager& AudioManager::GetInstance() noexcept {
	static AudioManager instance;
	return instance;
}

void AudioManager::Initialize() {
	auto&& instance = GetInstance();
	HRESULT result;
	result = XAudio2Create(instance.xAudio2.GetAddressOf(), 0, XAUDIO2_DEFAULT_PROCESSOR);
	result = instance.xAudio2->CreateMasteringVoice(&instance.masteringVoice);
	// nullインスタンスの追加
	Transfer("NULL", nullptr);

}

void AudioManager::Finalize() {
	auto&& instance = GetInstance();
	instance.xAudio2.Reset();
	instance.audioResources.clear();
}

void AudioManager::RegisterLoadQue(const std::string& filePath, const std::string& audioFile) {
	if (IsRegistered(audioFile)) {
		return;
	}
	BackgroundLoader::RegisterLoadQue(LoadEvent::LoadAudio, filePath, audioFile);

}

const std::unique_ptr<AudioResource>& AudioManager::GetAudio(const std::string& audioName) {
	std::lock_guard<std::mutex> lock{ audioMutex };
	// 見つかったらそのデータのweak_ptrを返す
	if (IsRegisteredNolocking(audioName)) {
		return GetInstance().audioResources.at(audioName);
	}
	else {
		Log(std::format("[AudioManager] Audio Name-\'{:}\' is not loading.\n", audioName));
		return GetInstance().audioResources.at("NULL");
	}
}

bool AudioManager::IsRegistered(const std::string& audioName) noexcept(false) {
	std::lock_guard<std::mutex> lock{ audioMutex };
	return IsRegisteredNolocking(audioName);
}

void AudioManager::UnloadAudio(const std::string& audioName) {
	std::lock_guard<std::mutex> lock{ audioMutex };
	if (IsRegisteredNolocking(audioName)) {
		GetInstance().audioResources.erase(audioName);
	}
}

void AudioManager::Transfer(const std::string& name, std::unique_ptr<AudioResource>&& data) {
	std::lock_guard<std::mutex> lock{ audioMutex };
	if (IsRegisteredNolocking(name)) {
		Log(std::format("[AudioManager] Transferring registered Audio. Name-\'{:}\', Address-\'{:}\'\n", name, (void*)data.get()));
		return;
	}
	Log(std::format("[AudioManager] Transfer new Audio. Name-\'{:}\', Address-\'{:}\'\n", name, (void*)data.get()));
	GetInstance().audioResources.emplace(name, std::move(data));

}

bool AudioManager::IsRegisteredNolocking(const std::string& audioName) noexcept(false) {
	return GetInstance().audioResources.contains(audioName);
}
