#include "AudioAssetBuilder.h"

#include "./AudioAsset.h"
#include "./AudioLibrary.h"

#include <Library/Utility/Tools/SmartPointer.h>

AudioAssetBuilder::AudioAssetBuilder(const std::filesystem::path& filePath_) {
	filePath = filePath_;
}

void AudioAssetBuilder::preprocess() {
	audioData = eps::CreateUnique<AudioAsset>();
}

bool AudioAssetBuilder::run() {
	return audioData->load(filePath);
}

void AudioAssetBuilder::postprocess() {
	// Do nothing
}

void AudioAssetBuilder::transfer() {
	AudioLibrary::Transfer(filePath.filename().string(), std::move(audioData));
}
