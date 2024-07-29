#pragma once

#include <numbers>
#include <utility>

constexpr float PI = float(std::numbers::pi);
constexpr float PI_H = float(std::numbers::pi * 0.5);
constexpr float PI2 = float(std::numbers::pi * 2.0);

constexpr float ToRadian = PI / 180.0f;
constexpr float ToDegree = 180.0f / PI;

/// <summary>
/// メモリのdeleteとnullptr化
/// <param name="mem">開放するメモリの変数</param>
/// </summary>
template<class type>
constexpr void MemClear(type*& mem);

///-------------------///
///   ここから定義     ///
///-------------------///

template<class type>
inline constexpr void MemClear(type*& mem) {
	delete mem;
	mem = nullptr;
}

class Animation {
public:
	Animation();
	Animation(int distance_, int total_seq_, bool is_loop_);

public:
	int now(int time) const {
		return is_loop ? time / distance % total_seq : (std::min)(time / distance, total_seq);
	}

	bool is_end(int time) const {
		return time / distance >= total_seq;
	}

public:
	int distance;
	int total_seq;
	bool is_loop;
};