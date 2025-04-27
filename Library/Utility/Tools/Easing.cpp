#include "Easing.h"

#include <cmath>

#include "Library/Math/Definition.h"

//----------------In----------------
float Easing::In::Sine(float t) noexcept {
	return 1.0f - std::cos(PI_H * t);
}

float Easing::In::Quad(float t) noexcept {
	return t * t;
}

float Easing::In::Cubic(float t) noexcept {
	return t * t * t;
}

float Easing::In::Quart(float t) noexcept {
	return t * t * t * t;
}

float Easing::In::Quint(float t) noexcept {
	return t * t * t * t * t;
}

float Easing::In::Expo(float t) noexcept {
	return t == 0 ? 0.0f : std::pow(2.0f, t * 10.0f - 10.0f);
}

float Easing::In::Circ(float t) noexcept {
	return 1.0f - std::sqrt(1.0f - t * t);
}

float Easing::In::Back(float t) noexcept {
	return 2.70158f * t * t * t - 1.70158f * t * t;
}

float Easing::In::Elastic(float t) noexcept {
	return t == 0 ? 0.0f
		: t == 1.0f ? 1.0f
		: -std::pow(2.0f, 10.0f * t - 10.0f) * std::sin((t * 10.0f - 10.75f) * PI2 / 3);
}

float Easing::In::Bounce(float t) noexcept {
	return 1 - Easing::Out::Bounce(1 - t);
}

//----------------Out----------------
float Easing::Out::Sine(float t) noexcept {
	return std::sin(PI_H * t);
}

float Easing::Out::Quad(float t) noexcept {
	return 1 - Easing::In::Quad(1 - t);
}

float Easing::Out::Cubic(float t) noexcept {
	return 1 - Easing::In::Cubic(1 - t);
}

float Easing::Out::Quart(float t) noexcept {
	return 1 - Easing::In::Quart(1 - t);
}

float Easing::Out::Quint(float t) noexcept {
	return 1 - Easing::In::Quint(1 - t);
}

float Easing::Out::Expo(float t) noexcept {
	return t == 1.0f ? 1.0f : 1.0f - std::pow(2.0f, t * -10.0f);
}

float Easing::Out::Circ(float t) noexcept {
	return std::sqrt(1.0f - std::pow(t - 1.0f, 2.0f));
}

float Easing::Out::Back(float t) noexcept {
	return 1 + 2.70158f * (t - 1) * (t - 1) * (t - 1) + 1.70158f * (t - 1) * (t - 1);
}

float Easing::Out::Elastic(float t) noexcept {
	return t == 0 ? 0
		: t == 1 ? 1
		: std::pow(2.0f, -10.0f * t) * std::sin((t * 10.0f - 0.75f) * PI2 / 3.0f) + 1.0f;
	;
}

float Easing::Out::Bounce(float t) noexcept {
	if (t < (1 / 2.75f)) {
		return 7.5625f * t * t;
	}
	else if (t < 2 / 2.75f) {
		return 7.5625f * (t -= 1.5f / 2.75f) * t + 0.75f;
	}
	else if (t < 2.5 / 2.75f) {
		return 7.5625f * (t -= 2.25f / 2.75f) * t + 0.9375f;
	}
	else {
		return 7.5625f * (t -= 2.625f / 2.75f) * t + 0.984375f;
	};
}

//----------------InOut----------------

float Easing::InOut::Sine(float t) noexcept {
	return -(std::cos(PI * t) - 1) / 2;
}

float Easing::InOut::Quad(float t) noexcept {
	return t < 0.5 ? 2 * Easing::In::Quad(t) : 1 - Easing::In::Quad(-2 * t + 2) / 2;
}

float Easing::InOut::Cubic(float t) noexcept {
	return t < 0.5 ? 4 * Easing::In::Cubic(t) : 1 - Easing::In::Cubic(-2 * t + 2) / 2;
}

float Easing::InOut::Quart(float t) noexcept {
	return t < 0.5 ? 8 * Easing::In::Quart(t) : 1 - Easing::In::Quart(-2 * t + 2) / 2;
}

float Easing::InOut::Quint(float t) noexcept {
	return t < 0.5 ? 16 * Easing::In::Quint(t) : 1 - Easing::In::Quint(-2 * t + 2) / 2;
}

float Easing::InOut::Expo(float t) noexcept {
	return t < 0.5 ? Easing::In::Expo(2 * t) / 2 : (Easing::Out::Expo(2 * t - 1) + 1) / 2;
}

float Easing::InOut::Circ(float t) noexcept {
	return t < 0.5 ? Easing::In::Circ(t * 2) / 2 : (Easing::Out::Circ(-2 * t + 3) + 1) / 2;
}

float Easing::InOut::Back(float t) noexcept {
	return t < 0.5f ? (std::pow(2 * t, 2.0f) * ((2.5949095f + 1) * 2 * t - 2.5949095f)) / 2
		: (std::pow(2 * t - 2, 2.0f) * ((2.5949095f + 1) * (t * 2 - 2) + 2.5949095f) + 2) / 2;
}

float Easing::InOut::Elastic(float t) noexcept {
	return t == 0 ? 0.0f
		: t == 1.0f ? 1.0f
		: t < 0.5f
		? -(std::pow(2.0f, 20 * t - 10) * std::sin((20 * t - 11.125f) * PI2 / 4.5f)) / 2
		: (std::pow(2.0f, -20 * t + 10) * std::sin((20 * t - 11.125f) * PI2 / 4.5f)) / 2 +
		1.0f;
}

float Easing::InOut::Bounce(float t) noexcept {
	return t < 0.5 ? (1 - Easing::Out::Bounce(1 - 2 * t)) / 2
		: (1 + Easing::Out::Bounce(2 * t - 1)) / 2;
}
