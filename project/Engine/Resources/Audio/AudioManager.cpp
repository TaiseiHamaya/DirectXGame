#include "AudioManager.h"

#pragma comment(lib, "xaudio2.lib")

#include <mutex>

#include "Engine/Resources/BackgroundLoader/BackgroundLoader.h"
#include "Engine/Resources/Audio/AudioResource.h"
#include "Engine/Debug/Output.h"

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
	instance.masteringVoice->GetVolume(&instance.masterVolume);
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
		Console("[AudioManager] Audio Name-\'{:}\' is not loading.\n", audioName);
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
		Console("[AudioManager] Transferring registered Audio. Name-\'{:}\', Address-\'{:}\'\n", name, (void*)data.get());
		return;
	}
	Console("[AudioManager] Transfer new Audio. Name-\'{:}\', Address-\'{:}\'\n", name, (void*)data.get());
	GetInstance().audioResources.emplace(name, std::move(data));

}

float AudioManager::GetMasterVolume() {
	return GetInstance().masterVolume;
}

void AudioManager::SetMasterVolume(float volume) {
	auto&& instance = GetInstance();
	instance.masterVolume = volume;
	instance.masteringVoice->SetVolume(instance.masterVolume);
}

#ifdef _DEBUG

#include <imgui.h>
void AudioManager::DebugGui() {
	ImGui::Begin("AudioManager");
	if (ImGui::DragFloat("Volume", &GetInstance().masterVolume, 0.01f, 0.0f, 100.0f, "%.2f")) {
		SetMasterVolume(GetInstance().masterVolume);
	}
	ImGui::End();
}
#endif // _DEBUG

bool AudioManager::IsRegisteredNolocking(const std::string& audioName) noexcept(false) {
	return GetInstance().audioResources.contains(audioName);
}
