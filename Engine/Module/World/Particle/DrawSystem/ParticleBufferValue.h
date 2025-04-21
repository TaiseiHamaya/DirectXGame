#pragma once

#include <Library/Math/Color4.h>
#include <Library/Math/Matrix4x4.h>

struct ParticleBuffer {
	Matrix4x4 world{ CMatrix4x4::IDENTITY };
	Matrix4x4 uvMatrix{ CMatrix4x4::IDENTITY };
	Color4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
};
