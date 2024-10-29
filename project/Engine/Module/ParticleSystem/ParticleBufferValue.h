#pragma once

#include "Library/Math/Matrix4x4.h"
#include "Library/Math/Color.h"

struct ParticleBuffer {
	Matrix4x4 world{ CMatrix4x4::IDENTITY };
	Color color{ 1.0f, 1.0f, 1.0f, 1.0f };
};
