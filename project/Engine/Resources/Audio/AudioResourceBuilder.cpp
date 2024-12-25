#include "AudioResourceBuilder.h"

#include "./AudioManager.h"
#include "./AudioResource.h"
#include "Engine/Utility/Tools/SmartPointer.h"

AudioResourceBuilder::AudioResourceBuilder(const std::filesystem::path& filePath_) {
	filePath = filePath_;
}

void AudioResourceBuilder::preprocess() {
	audioData = eps::CreateUnique<AudioResource>();
}

bool AudioResourceBuilder::run() {
	return audioData->load(filePath);
}

void AudioResourceBuilder::postprocess() {
	// Do nothing
}

void AudioResourceBuilder::transfer() {
	AudioManager::Transfer(filePath.filename().string(), std::move(audioData));
}
