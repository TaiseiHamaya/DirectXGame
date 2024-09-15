#pragma once

#include <numbers>

constexpr float PI = float(std::numbers::pi);
constexpr float PI_H = float(std::numbers::pi * 0.5);
constexpr float PI2 = float(std::numbers::pi * 2.0);

constexpr float ToRadian = PI / 180.0f;
constexpr float ToDegree = 180.0f / PI;