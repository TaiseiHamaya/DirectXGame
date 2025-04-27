#pragma once

#include "MathEPS.h"

// 参考URL
// https://qiita.com/oishihiroaki/items/9d899cdcb9bee682531a#smoothstep
// https://moge32.blogspot.com/2013/11/gen-waveform.html

namespace eps {

/// <summary>
/// value >= separate ? 1 : 0;
/// </summary>
/// <returns></returns>
constexpr float step(float separate, float value) noexcept;

/// <summary>
/// [off, off+on)間で1になるパルス関数
/// </summary>
constexpr float pulse_once(float off, float on, float param) noexcept;

/// <summary>
/// 短径波
/// </summary>
/// <param name="off">0の区間長</param>
/// <param name="on">1の区間長</param>
/// <param name="param"></param>
/// <returns></returns>
constexpr float pulse(float off, float on, float param) noexcept;

/// <summary>
/// のこぎり波[0-1)
/// </summary>
/// <param name="period">周期</param>
/// <param name="param"></param>
/// <returns></returns>
constexpr float sawtooth(float period, float param) noexcept;

/// <summary>
/// 三角波
/// </summary>
/// <param name="up">0->1になる区間長</param>
/// <param name="down">1->0になる区間長</param>
/// <param name="param"></param>
/// <returns></returns>
constexpr float triangle(float up, float down, float param) noexcept;

/// <summary>
/// Param == Start、End の時傾きが0
/// Param == Start->0、Param == End->1
/// となる 3x^2 - 2x^3
/// </summary>
/// <returns></returns>
constexpr float smoothstep(float start, float end, float param) noexcept;

}

constexpr float eps::step(float separate, float value) noexcept {
	return value >= separate ? 1.0f : 0.0f;
}

constexpr float eps::pulse_once(float start, float end, float param) noexcept {
	if (param >= start && param < end) {
		return 1.0f;
	}
	else {
		return 0.0f;
	}
}

constexpr float eps::pulse(float off, float on, float param) noexcept {
	float period = off + on;
	float t = eps::sawtooth(period, param);
	return eps::pulse_once(off, period, t);
}


constexpr float eps::sawtooth(float period, float param) noexcept {
	float t = eps::lerp_inv(0.0f, period, param);
	return eps::fractional(t);
}

constexpr float eps::triangle(float up, float down, float param) noexcept {
	float period = up + down;
	float t = eps::sawtooth(period, param);
	if (t < up / period) {
		return t * 2.0f / up;
	}
	else {
		return 2.0f * (1.0f - t) / down;
	}
}

constexpr float eps::smoothstep(float start, float end, float param) noexcept {
	float t = eps::saturate((param - start) / (end - start));
	return t * t * (3.0f - 2.0f * t);
}
