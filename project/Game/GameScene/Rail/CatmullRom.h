#pragma once

#include <cmath>
#include <vector>
#include <cassert>

template<class Type>
inline Type CatmullRom(const Type& p0, const Type& p1, const Type& p2, const Type& p3, float t) {
	return (
		(-p0 + p1 * 3 - p2 * 3 + p3) * t * t * t +
		(p0 * 2 - p1 * 5 + p2 * 4 - p3) * t * t +
		(-p0 + p2) * t +
		p1 * 2) / 2;
}

float CatmullRomWidth(float t, uint32_t size) {
	uint32_t division = size - 1;

	return std::fmod(t * division, 1.0f) + static_cast<int32_t>(t);
}

template<class Type>
inline Type CatmullRom(std::vector<Type>& points, float t) {
	assert(points.size() >= 4);
	uint32_t division = static_cast<std::uint32_t> (points.size() - 1);
	float interveningWidth = CatmullRomWidth(t, uint32_t(points.size()));

	uint32_t baseIndex = t < 1 ? static_cast<uint32_t>(t * division) : division - 1;

	uint32_t index0 = baseIndex != 0 ? baseIndex - 1 : 0;
	uint32_t index1 = baseIndex;
	uint32_t index2 = baseIndex + 1;
	uint32_t index3 = index2 < division ? baseIndex + 2 : index2;

	return CatmullRom(points[index0], points[index1], points[index2], points[index3], interveningWidth);
}