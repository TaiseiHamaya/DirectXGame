#pragma once

#include <vector>

#include <xaudio2.h>

class AudioAsset {
public:
	AudioAsset(WAVEFORMATEXTENSIBLE format, std::vector<u8> buffer, u32 bufferSize);
	~AudioAsset() = default;

public:
	AudioAsset(const AudioAsset&) = delete;
	AudioAsset& operator=(const AudioAsset&) = delete;
	AudioAsset(AudioAsset&&) = default;
	AudioAsset& operator=(AudioAsset&&) = default;

public:
	u32 size() const noexcept;
	const std::vector<u8>& buffer_data() const noexcept;
	const WAVEFORMATEX& format() const noexcept;

private:
	WAVEFORMATEXTENSIBLE format_;
	std::vector<u8> buffer_;
	u32 bufferSize_;
};

