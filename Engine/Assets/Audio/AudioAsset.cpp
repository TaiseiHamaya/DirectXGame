#include "AudioAsset.h"

AudioAsset::AudioAsset(WAVEFORMATEXTENSIBLE _format, std::vector<u8> _buffer, u32 _bufferSize) :
	format_(_format),
	buffer_(_buffer),
	bufferSize_(_bufferSize) {
}

u32 AudioAsset::size() const noexcept {
	return bufferSize_;
}

const std::vector<u8>& AudioAsset::buffer_data() const noexcept {
	return buffer_;
}

const WAVEFORMATEX& AudioAsset::format() const noexcept {
	return format_.Format;
}
