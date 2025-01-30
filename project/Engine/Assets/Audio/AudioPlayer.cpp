#include "AudioPlayer.h"

#include <cassert>

#include "Engine/Assets/Audio/AudioAsset.h"
#include "Engine/Assets/Audio/AudioLibrary.h"

AudioPlayer::~AudioPlayer() {
	destroy();
}

void AudioPlayer::initialize(const std::string& name, float volume, bool isLoop) {
	auto&& audio = AudioLibrary::GetAudio(name);
	if (!audio) {
		return;
	}
	HRESULT result;
	result = AudioLibrary::GetXAudio2()->CreateSourceVoice(&sourceVoice, &audio->format());
	assert(SUCCEEDED(result));

	buffer.pAudioData = audio->buffer_data().data();
	buffer.AudioBytes = audio->size();
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = isLoop ? XAUDIO2_LOOP_INFINITE : 0;
	sourceVoice->SetVolume(volume);
	result = sourceVoice->SubmitSourceBuffer(&buffer);
	assert(SUCCEEDED(result));
}

void AudioPlayer::destroy() {
	if (sourceVoice) {
		pause();
		sourceVoice->DestroyVoice();
	}
	sourceVoice = nullptr;
}

void AudioPlayer::play() {
	if (!sourceVoice) {
		return;
	}
	HRESULT result;
	result = sourceVoice->Start();
	assert(SUCCEEDED(result));
}

void AudioPlayer::stop() {
	if (!sourceVoice) {
		return;
	}
	HRESULT result;
	result = sourceVoice->Stop();
	assert(SUCCEEDED(result));
	sourceVoice->FlushSourceBuffers();
	sourceVoice->SubmitSourceBuffer(&buffer);
}

void AudioPlayer::pause() {
	if (!sourceVoice) {
		return;
	}
	HRESULT result;
	result = sourceVoice->Stop();
	assert(SUCCEEDED(result));
}

void AudioPlayer::restart() {
	stop();
	play();
}

void AudioPlayer::set_volume(float volume) {
	if (!sourceVoice) {
		return;
	}
	sourceVoice->SetVolume(volume);
}

void AudioPlayer::set_loop(bool isLoop) {
	if (!sourceVoice) {
		return;
	}
	buffer.LoopCount = isLoop ? XAUDIO2_LOOP_INFINITE : 0;
	HRESULT result;
	result = sourceVoice->SubmitSourceBuffer(&buffer);
}

#ifdef _DEBUG
#include <imgui.h>
void AudioPlayer::debug_gui() {
	if (!sourceVoice) {
		return;
	}
	bool isLoop = buffer.LoopCount == XAUDIO2_LOOP_INFINITE;
	if (ImGui::Checkbox("Loop", &isLoop)) {
		set_loop(isLoop);
	}
	float volume;
	sourceVoice->GetVolume(&volume);
	if (ImGui::DragFloat("Volume", &volume, 0.01f, 0.0f, 100.0f)) {
		set_volume(volume);
	}
}
#endif // _DEBUG
