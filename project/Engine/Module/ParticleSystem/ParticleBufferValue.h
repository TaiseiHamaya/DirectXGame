#pragma once

#include "Engine/Math/Matrix4x4.h"
#include "Engine/Module/Color/Color.h"

struct ParticleBuffer {
	Matrix4x4 world{ CMatrix4x4::IDENTITY };
	Color color{ 1.0f, 1.0f, 1.0f, 1.0f };
	uint32_t isDraw = 0;
};
