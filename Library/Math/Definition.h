#pragma once

#include <numbers>

constexpr r32 PI = r32(std::numbers::pi);
constexpr r32 PI_H = r32(std::numbers::pi * 0.5);
constexpr r32 PI2 = r32(std::numbers::pi * 2.0);

constexpr r32 ToRadian = PI / 180.0f;
constexpr r32 ToDegree = 180.0f / PI;