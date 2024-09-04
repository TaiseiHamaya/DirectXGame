#pragma once

#include <wrl/client.h>
#include <string>
#include <xaudio2.h>

class AudioResource;

class AudioPlayer {
public:
	AudioPlayer() = default;
	~AudioPlayer();

public:
	AudioPlayer(const AudioPlayer&) = delete;
	AudioPlayer& operator=(const AudioPlayer&) = delete;
	AudioPlayer(AudioPlayer&&) = default;
	AudioPlayer& operator=(AudioPlayer&&) = default;

public:
	void initialize(const std::string& name, float volume = 1.0f, bool isLoop = false);
	void finalize();
	void play();
	void stop();
	void pause();
	//bool is_play();

	void set_volume(float volume);
	void set_loop(bool isLoop);

private:
	XAUDIO2_BUFFER buffer{};
	IXAudio2SourceVoice* sourceVoice;
};
