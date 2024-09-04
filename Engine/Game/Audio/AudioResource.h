#pragma once

#include <string>

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
	bool load(const std::string& filePath, const std::string& fileName);
	std::uint32_t size() const noexcept;
	const std::basic_string<BYTE>& buffer_data() const;
	const  WAVEFORMATEX& format() const;

private:
	WAVEFORMATEX format_;
	std::basic_string<BYTE> buffer_;
	std::uint32_t bufferSize_;
};

