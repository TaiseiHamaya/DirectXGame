#include "AudioLibrary.h"

#pragma comment(lib, "xaudio2.lib")

#include <mutex>

#include "./AudioAsset.h"
#include "./AudioAssetBuilder.h"
#include "Engine/Assets/BackgroundLoader/BackgroundLoader.h"
#include "Library/Utility/Tools/SmartPointer.h"
#include "Engine/Debug/Output.h"

std::mutex audioMutex;

AudioLibrary::AudioLibrary() noexcept = default;

AudioLibrary::~AudioLibrary() noexcept = default;

AudioLibrary& AudioLibrary::GetInstance() noexcept {
	static AudioLibrary instance;
	return instance;
}

void AudioLibrary::Initialize() {
	auto&& instance = GetInstance();
	HRESULT result;
	result = XAudio2Create(instance.xAudio2.GetAddressOf(), 0, XAUDIO2_DEFAULT_PROCESSOR);
	result = instance.xAudio2->CreateMasteringVoice(&instance.masteringVoice);
	instance.masteringVoice->GetVolume(&instance.masterVolume);
	// nullインスタンスの追加
	Transfer("NULL", nullptr);
}

void AudioLibrary::Finalize() {
	auto&& instance = GetInstance();
	instance.xAudio2.Reset();
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
		Console("Warning : Audio Name-\'{:}\' is not loading.\n", audioName);
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
		Console("Warning : Transferring registered Audio. Name-\'{:}\', Address-\'{:}\'\n", name, (void*)data.get());
		return;
	}
	Console("Transfer new Audio. Name-\'{:}\', Address-\'{:}\'\n", name, (void*)data.get());
	GetInstance().audioResources.emplace(name, std::move(data));

}

float AudioLibrary::GetMasterVolume() {
	return GetInstance().masterVolume;
}

void AudioLibrary::SetMasterVolume(float volume) {
	auto&& instance = GetInstance();
	instance.masterVolume = volume;
	instance.masteringVoice->SetVolume(instance.masterVolume);
}

#ifdef _DEBUG

#include <imgui.h>
void AudioLibrary::DebugGui() {
	ImGui::Begin("AudioLibrary");
	if (ImGui::DragFloat("Volume", &GetInstance().masterVolume, 0.01f, 0.0f, 100.0f, "%.2f")) {
		SetMasterVolume(GetInstance().masterVolume);
	}
	ImGui::End();
}
#endif // _DEBUG

bool AudioLibrary::IsRegisteredNonlocking(const std::string& audioName) noexcept(false) {
	return GetInstance().audioResources.contains(audioName);
}
