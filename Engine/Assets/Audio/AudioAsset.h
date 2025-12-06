#pragma once

#include <vector>

#include <xaudio2.h>

#include <Library/Utility/Tools/ConstructorMacro.h>

/// <summary>
/// 音声データアセット
/// </summary>
namespace szg {

class AudioAsset {
public:
	AudioAsset(WAVEFORMATEXTENSIBLE format, std::vector<u8> buffer, u32 bufferSize);
	~AudioAsset() = default;

	SZG_CLASS_MOVE_ONLY(AudioAsset)

public:
	u32 buffer_size() const noexcept;
	const std::vector<u8>& buffer_data() const noexcept;
	const WAVEFORMATEX& format() const noexcept;

private:
	WAVEFORMATEXTENSIBLE waveFormat;
	std::vector<u8> buffer;
	u32 bufferSize;
};

}; // szg
