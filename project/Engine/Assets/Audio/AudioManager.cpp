#include "AudioManager.h"

#pragma comment(lib, "xaudio2.lib")

#include "./AudioLibrary.h" 

void AudioManager::Initialize() {
	auto& instance = GetInstance();
	HRESULT result;
	result = XAudio2Create(instance.xAudio2.GetAddressOf(), 0, XAUDIO2_DEFAULT_PROCESSOR);
	result = instance.xAudio2->CreateMasteringVoice(&instance.masteringVoice);
	instance.masteringVoice->GetVolume(&instance.masterVolume);

	AudioLibrary::Initialize();
}

void AudioManager::Finalize() {
	auto& instance = GetInstance();
	AudioLibrary::Finalize();
	instance.xAudio2.Reset();
}

#ifdef _DEBUG
#include <imgui.h>
void AudioManager::DebugGui() {
	ImGui::Begin("AudioLibrary");
	if (ImGui::DragFloat("Volume", &GetInstance().masterVolume, 0.01f, 0.0f, 100.0f, "%.2f")) {
		SetMasterVolume(GetInstance().masterVolume);
	}
	ImGui::End();
}
#endif // _DEBUG

float AudioManager::GetMasterVolume() {
	return GetInstance().masterVolume;
}

void AudioManager::SetMasterVolume(float volume) {
	auto&& instance = GetInstance();
	instance.masterVolume = volume;
	instance.masteringVoice->SetVolume(instance.masterVolume);
}
