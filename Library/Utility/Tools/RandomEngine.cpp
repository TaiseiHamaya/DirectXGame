#include "RandomEngine.h"

#include "Engine/Application/Output.h"

RandomEngine& RandomEngine::GetInstance() {
	static RandomEngine instance;
	return instance;
}

void RandomEngine::Initialize() {
	RandomEngine& instance = GetInstance();
	std::random_device device{};
	u32 seed = device();
#ifdef DEBUG_FEATURES_ENABLE
	Information("RandomEngine is initalized. Seed-\'{:#x}\'", seed);
#else
	Information("RandomEngine is initalized. SeedHash-\'{:#x}\'", std::hash<u32>()(seed));
#endif // _DEBUG
	instance.mersenneTwister.seed(seed);
}

u32 RandomEngine::RandomMT() {
	RandomEngine& instance = GetInstance();
	return instance.mersenneTwister();
}

r32 RandomEngine::Random01() {
	RandomEngine& instance = GetInstance();
	return instance.random01(instance.mersenneTwister);
}

r32 RandomEngine::Random01MOD() {
	// なんかしらんけどこれでうまくいくらしい
	// ソース : https://speakerdeck.com/hole/rand01?slide=42
	return (RandomMT() >> 8) / 16777216.0f;
}

r32 RandomEngine::Random01Closed() {
	RandomEngine& instance = GetInstance();
	return instance.random01Closed(instance.mersenneTwister);
}

r32 RandomEngine::RandomPIClosed() {
	RandomEngine& instance = GetInstance();
	return instance.randomPI(instance.mersenneTwister);
}
