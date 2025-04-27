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
constexpr r32 step(r32 separate, r32 value) noexcept;

/// <summary>
/// [off, off+on)間で1になるパルス関数
/// </summary>
constexpr r32 pulse_once(r32 off, r32 on, r32 param) noexcept;

/// <summary>
/// 短径波
/// </summary>
/// <param name="off">0の区間長</param>
/// <param name="on">1の区間長</param>
/// <param name="param"></param>
/// <returns></returns>
constexpr r32 pulse(r32 off, r32 on, r32 param) noexcept;

/// <summary>
/// のこぎり波[0-1)
/// </summary>
/// <param name="period">周期</param>
/// <param name="param"></param>
/// <returns></returns>
constexpr r32 sawtooth(r32 period, r32 param) noexcept;

/// <summary>
/// 三角波
/// </summary>
/// <param name="up">0->1になる区間長</param>
/// <param name="down">1->0になる区間長</param>
/// <param name="param"></param>
/// <returns></returns>
constexpr r32 triangle(r32 up, r32 down, r32 param) noexcept;

/// <summary>
/// Param == Start、End の時傾きが0
/// Param == Start->0、Param == End->1
/// となる 3x^2 - 2x^3
/// </summary>
/// <returns></returns>
constexpr r32 smoothstep(r32 start, r32 end, r32 param) noexcept;

}

constexpr r32 eps::step(r32 separate, r32 value) noexcept {
	return value >= separate ? 1.0f : 0.0f;
}

constexpr r32 eps::pulse_once(r32 start, r32 end, r32 param) noexcept {
	if (param >= start && param < end) {
		return 1.0f;
	}
	else {
		return 0.0f;
	}
}

constexpr r32 eps::pulse(r32 off, r32 on, r32 param) noexcept {
	r32 period = off + on;
	r32 t = eps::sawtooth(period, param);
	return eps::pulse_once(off, period, t);
}


constexpr r32 eps::sawtooth(r32 period, r32 param) noexcept {
	r32 t = eps::lerp_inv(0.0f, period, param);
	return eps::fractional(t);
}

constexpr r32 eps::triangle(r32 up, r32 down, r32 param) noexcept {
	r32 period = up + down;
	r32 t = eps::sawtooth(period, param);
	if (t < up / period) {
		return t * 2.0f / up;
	}
	else {
		return 2.0f * (1.0f - t) / down;
	}
}

constexpr r32 eps::smoothstep(r32 start, r32 end, r32 param) noexcept {
	r32 t = eps::saturate((param - start) / (end - start));
	return t * t * (3.0f - 2.0f * t);
}
