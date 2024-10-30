#include "AudioPlayer.h"

#include <cassert>

#include "Engine/Resources/Audio/AudioResource.h"
#include "Engine/Resources/Audio/AudioManager.h"

AudioPlayer::~AudioPlayer() {
	//finalize();
}

void AudioPlayer::initialize(const std::string& name, float volume, bool isLoop) {
	auto&& audio = AudioManager::GetAudio(name);
	if (!audio) {
		return;
	}
	HRESULT result;
	result = AudioManager::GetXAudio2()->CreateSourceVoice(&sourceVoice, &audio->format());
	assert(SUCCEEDED(result));

	buffer.pAudioData = audio->buffer_data().data();
	buffer.AudioBytes = audio->size();
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = isLoop ? XAUDIO2_LOOP_INFINITE : 0;
	sourceVoice->SetVolume(volume);
	result = sourceVoice->SubmitSourceBuffer(&buffer);
	assert(SUCCEEDED(result));
}

void AudioPlayer::finalize() {
	if (sourceVoice) {
		pause();
		sourceVoice->DestroyVoice();
	}
	sourceVoice = nullptr;
}

void AudioPlayer::play() {
	HRESULT result;
	result = sourceVoice->Start();
	assert(SUCCEEDED(result));
}

void AudioPlayer::stop() {
	HRESULT result;
	result = sourceVoice->Stop();
	assert(SUCCEEDED(result));
	sourceVoice->FlushSourceBuffers();
	sourceVoice->SubmitSourceBuffer(&buffer);
}

void AudioPlayer::pause() {
	HRESULT result;
	result = sourceVoice->Stop();
	assert(SUCCEEDED(result));
}

void AudioPlayer::restart() {
	stop();
	play();
}

void AudioPlayer::set_volume(float volume) {
	sourceVoice->SetVolume(volume);
}

void AudioPlayer::set_loop(bool isLoop) {
	buffer.LoopCount = isLoop ? XAUDIO2_LOOP_INFINITE : 0;
	HRESULT result;
	result = sourceVoice->SubmitSourceBuffer(&buffer);
}

#ifdef _DEBUG
#include <imgui.h>
void AudioPlayer::debug_gui() {
	bool isLoop = buffer.LoopCount == XAUDIO2_LOOP_INFINITE;
	if(ImGui::Checkbox("Loop", &isLoop) ){
		set_loop(isLoop);
	}
	float volume;
	sourceVoice->GetVolume(&volume);
	if (ImGui::DragFloat("Volume", &volume, 0.01f, 0.0f, 100.0f)) {
		set_volume(volume);
	}
}
#endif // _DEBUG
