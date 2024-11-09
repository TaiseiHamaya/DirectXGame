#pragma once

#include <cstdint>
#include <random>
#include <type_traits>

#include "Library/Math/Definition.h"

class RandomEngine {
private:
	RandomEngine() = default;
	~RandomEngine() = default;

	RandomEngine(const RandomEngine&) = delete;
	RandomEngine& operator=(const RandomEngine&) = delete;

public:
	static RandomEngine& GetInstance();

public:
	/// <summary>
	/// 初期化
	/// </summary>
	static void Initialize();

	/// <summary>
	/// メルセンヌ・ツイスターでの32bit乱数生成
	/// </summary>
	/// <returns></returns>
	static std::uint32_t RandomMT();

	/// <summary>
	/// [0-1)離散一様分布乱数を生成
	/// uniform_real_distribution(精度悪め)
	/// 詳細はここ : https://speakerdeck.com/hole/rand01?slide=66
	/// </summary>
	/// <returns></returns>
	static float Random01();

	/// <summary>
	/// 除算法による離散一様分布乱数
	/// [0-1)が保証されるので推奨
	/// </summary>
	/// <returns></returns>
	static float Random01MOD();

	/// <summary>
	/// [0-1]離散一様分布乱数を生成
	/// </summary>
	/// <returns></returns>
	static float Random01Closed();

	/// <summary>
	/// [-PI-PI]の離散一様分布乱数
	/// </summary>
	/// <returns></returns>
	static float RandomPIClosed();

	/// <summary>
	/// 0or1のランダムを出力
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns></returns>
	template<typename T>
		requires(std::convertible_to<unsigned int, T>)
	static T Random01Bit();

private:
	std::mt19937 mersenneTwister; // メルセンヌ・ツイスタ
	std::uniform_real_distribution<float> random01{ 0.0f,1.0f }; // [0.0, 1.0)
	std::uniform_real_distribution<float> random01Closed{ 0.0f,std::nextafter(1.0f, 2.0f) }; // [0.0, 1.0]
	std::uniform_real_distribution<float> randomPI{ -PI,std::nextafter(PI, 4.0f) }; // [-PI, PI]
};

template<typename T>
	requires(std::convertible_to<unsigned int, T>)
inline T RandomEngine::Random01Bit() {
	return static_cast<T>(RandomMT() & 0b1);
}
