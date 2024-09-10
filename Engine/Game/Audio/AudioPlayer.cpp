#include "AudioPlayer.h"

#include <cassert>

#include "Engine/Game/Audio/AudioResource.h"
#include "Engine/Game/Managers/AudioManager/AudioManager.h"

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
	//HRESULT result;
	//result = sourceVoice->SubmitSourceBuffer(&buffer);
}
