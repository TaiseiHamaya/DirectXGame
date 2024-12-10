#pragma once

#include <filesystem>

#include <xaudio2.h>

class AudioResource {
public:
	AudioResource() = default;
	~AudioResource() = default;

public:
	AudioResource(const AudioResource&) = delete;
	AudioResource& operator=(const AudioResource&) = delete;
	AudioResource(AudioResource&&) = default;
	AudioResource& operator=(AudioResource&&) = default;

public:
	bool load(const std::filesystem::path& filePath);
	std::uint32_t size() const noexcept;
	const std::basic_string<BYTE>& buffer_data() const noexcept;
	const WAVEFORMATEX& format() const noexcept;

private:
	WAVEFORMATEXTENSIBLE format_;
	std::basic_string<BYTE> buffer_;
	std::uint32_t bufferSize_;
};

