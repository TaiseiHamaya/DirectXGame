#include "Easing.h"

#include <cmath>

#include "Library/Math/Definition.h"

//----------------In----------------
r32 Easing::In::Sine(r32 t) noexcept {
	return 1.0f - std::cos(PI_H * t);
}

r32 Easing::In::Quad(r32 t) noexcept {
	return t * t;
}

r32 Easing::In::Cubic(r32 t) noexcept {
	return t * t * t;
}

r32 Easing::In::Quart(r32 t) noexcept {
	return t * t * t * t;
}

r32 Easing::In::Quint(r32 t) noexcept {
	return t * t * t * t * t;
}

r32 Easing::In::Expo(r32 t) noexcept {
	return t == 0 ? 0.0f : std::pow(2.0f, t * 10.0f - 10.0f);
}

r32 Easing::In::Circ(r32 t) noexcept {
	return 1.0f - std::sqrt(1.0f - t * t);
}

r32 Easing::In::Back(r32 t) noexcept {
	return 2.70158f * t * t * t - 1.70158f * t * t;
}

r32 Easing::In::Elastic(r32 t) noexcept {
	return t == 0 ? 0.0f
		: t == 1.0f ? 1.0f
		: -std::pow(2.0f, 10.0f * t - 10.0f) * std::sin((t * 10.0f - 10.75f) * PI2 / 3);
}

r32 Easing::In::Bounce(r32 t) noexcept {
	return 1 - Easing::Out::Bounce(1 - t);
}

//----------------Out----------------
r32 Easing::Out::Sine(r32 t) noexcept {
	return std::sin(PI_H * t);
}

r32 Easing::Out::Quad(r32 t) noexcept {
	return 1 - Easing::In::Quad(1 - t);
}

r32 Easing::Out::Cubic(r32 t) noexcept {
	return 1 - Easing::In::Cubic(1 - t);
}

r32 Easing::Out::Quart(r32 t) noexcept {
	return 1 - Easing::In::Quart(1 - t);
}

r32 Easing::Out::Quint(r32 t) noexcept {
	return 1 - Easing::In::Quint(1 - t);
}

r32 Easing::Out::Expo(r32 t) noexcept {
	return t == 1.0f ? 1.0f : 1.0f - std::pow(2.0f, t * -10.0f);
}

r32 Easing::Out::Circ(r32 t) noexcept {
	return std::sqrt(1.0f - std::pow(t - 1.0f, 2.0f));
}

r32 Easing::Out::Back(r32 t) noexcept {
	return 1 + 2.70158f * (t - 1) * (t - 1) * (t - 1) + 1.70158f * (t - 1) * (t - 1);
}

r32 Easing::Out::Elastic(r32 t) noexcept {
	return t == 0 ? 0
		: t == 1 ? 1
		: std::pow(2.0f, -10.0f * t) * std::sin((t * 10.0f - 0.75f) * PI2 / 3.0f) + 1.0f;
	;
}

r32 Easing::Out::Bounce(r32 t) noexcept {
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

r32 Easing::InOut::Sine(r32 t) noexcept {
	return -(std::cos(PI * t) - 1) / 2;
}

r32 Easing::InOut::Quad(r32 t) noexcept {
	return t < 0.5 ? 2 * Easing::In::Quad(t) : 1 - Easing::In::Quad(-2 * t + 2) / 2;
}

r32 Easing::InOut::Cubic(r32 t) noexcept {
	return t < 0.5 ? 4 * Easing::In::Cubic(t) : 1 - Easing::In::Cubic(-2 * t + 2) / 2;
}

r32 Easing::InOut::Quart(r32 t) noexcept {
	return t < 0.5 ? 8 * Easing::In::Quart(t) : 1 - Easing::In::Quart(-2 * t + 2) / 2;
}

r32 Easing::InOut::Quint(r32 t) noexcept {
	return t < 0.5 ? 16 * Easing::In::Quint(t) : 1 - Easing::In::Quint(-2 * t + 2) / 2;
}

r32 Easing::InOut::Expo(r32 t) noexcept {
	return t < 0.5 ? Easing::In::Expo(2 * t) / 2 : (Easing::Out::Expo(2 * t - 1) + 1) / 2;
}

r32 Easing::InOut::Circ(r32 t) noexcept {
	return t < 0.5 ? Easing::In::Circ(t * 2) / 2 : (Easing::Out::Circ(-2 * t + 3) + 1) / 2;
}

r32 Easing::InOut::Back(r32 t) noexcept {
	return t < 0.5f ? (std::pow(2 * t, 2.0f) * ((2.5949095f + 1) * 2 * t - 2.5949095f)) / 2
		: (std::pow(2 * t - 2, 2.0f) * ((2.5949095f + 1) * (t * 2 - 2) + 2.5949095f) + 2) / 2;
}

r32 Easing::InOut::Elastic(r32 t) noexcept {
	return t == 0 ? 0.0f
		: t == 1.0f ? 1.0f
		: t < 0.5f
		? -(std::pow(2.0f, 20 * t - 10) * std::sin((20 * t - 11.125f) * PI2 / 4.5f)) / 2
		: (std::pow(2.0f, -20 * t + 10) * std::sin((20 * t - 11.125f) * PI2 / 4.5f)) / 2 +
		1.0f;
}

r32 Easing::InOut::Bounce(r32 t) noexcept {
	return t < 0.5 ? (1 - Easing::Out::Bounce(1 - 2 * t)) / 2
		: (1 + Easing::Out::Bounce(2 * t - 1)) / 2;
}
