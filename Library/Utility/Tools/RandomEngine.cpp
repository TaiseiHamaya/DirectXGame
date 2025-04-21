#include "RandomEngine.h"

#include "Engine/Application/Output.h"

RandomEngine& RandomEngine::GetInstance() {
	static RandomEngine instance;
	return instance;
}

void RandomEngine::Initialize() {
	RandomEngine& instance = GetInstance();
	std::random_device device{};
	uint32_t seed = device();
#ifdef DEBUG_FEATURES_ENABLE
	Infomation("RandomEngine is initalized. Seed-\'{:#x}\'", seed);
#else
	Infomation("RandomEngine is initalized. SeedHash-\'{:#x}\'", std::hash<uint32_t>()(seed));
#endif // _DEBUG
	instance.mersenneTwister.seed(seed);
}

uint32_t RandomEngine::RandomMT() {
	RandomEngine& instance = GetInstance();
	return instance.mersenneTwister();
}

float RandomEngine::Random01() {
	RandomEngine& instance = GetInstance();
	return instance.random01(instance.mersenneTwister);
}

float RandomEngine::Random01MOD() {
	// なんかしらんけどこれでうまくいくらしい
	// ソース : https://speakerdeck.com/hole/rand01?slide=42
	return (RandomMT() >> 8) / 16777216.0f;
}

float RandomEngine::Random01Closed() {
	RandomEngine& instance = GetInstance();
	return instance.random01Closed(instance.mersenneTwister);
}

float RandomEngine::RandomPIClosed() {
	RandomEngine& instance = GetInstance();
	return instance.randomPI(instance.mersenneTwister);
}
