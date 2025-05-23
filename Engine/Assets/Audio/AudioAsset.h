#pragma once

#include <filesystem>

#include <xaudio2.h>

class AudioAsset {
public:
	AudioAsset() = default;
	~AudioAsset() = default;

public:
	AudioAsset(const AudioAsset&) = delete;
	AudioAsset& operator=(const AudioAsset&) = delete;
	AudioAsset(AudioAsset&&) = default;
	AudioAsset& operator=(AudioAsset&&) = default;

public:
	bool load(const std::filesystem::path& filePath);
	u32 size() const noexcept;
	const std::basic_string<BYTE>& buffer_data() const noexcept;
	const WAVEFORMATEX& format() const noexcept;

private:
	WAVEFORMATEXTENSIBLE format_;
	std::basic_string<BYTE> buffer_;
	u32 bufferSize_;
};

