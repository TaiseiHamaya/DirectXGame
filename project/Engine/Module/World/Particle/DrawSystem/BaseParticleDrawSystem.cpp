#include "BaseParticleDrawSystem.h"

void BaseParticleDrawSystem::create_buffers(uint32_t bufferSize) {
	particleBuffer.initialize(bufferSize);
}

void BaseParticleDrawSystem::write_to_buffer(uint32_t index, const Matrix4x4& worldMatrix, const Matrix4x4& uvMatrix, const Color4& color) {
	if (index >= particleBuffer.size()) {
	}
	particleBuffer[index] = {
		worldMatrix,
		uvMatrix,
		color
	};
}
