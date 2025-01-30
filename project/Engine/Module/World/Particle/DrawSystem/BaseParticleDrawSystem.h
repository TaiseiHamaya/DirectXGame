#pragma once

#include "./ParticleBufferValue.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/StructuredBuffer/StructuredBuffer.h"

#include <Library/Utility/Tools/ConstructorMacro.h>

enum class ParticleDrawType {
	Mesh,
	Rect,
};

class BaseParticleDrawSystem {
public:
	BaseParticleDrawSystem() = default;
	virtual ~BaseParticleDrawSystem() = default;

	__NON_COPYABLE_CLASS(BaseParticleDrawSystem)

public:
	virtual void draw_command(size_t InstanceCount) const = 0;
	void create_buffers(uint32_t bufferSize);
	void write_to_buffer(uint32_t index, const Matrix4x4& worldMatrix, const Matrix4x4& uvMatrix, const Color4& color);

protected:
	StructuredBuffer<ParticleBuffer> particleBuffer;
};
