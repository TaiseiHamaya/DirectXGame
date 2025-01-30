#pragma once

#include "../BaseDrawExecutor.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/StructuredBuffer/StructuredBuffer.h"

struct DirectionalLightData;

class DirectionalLightingExecutor {
public:
	DirectionalLightingExecutor() = default;
	virtual ~DirectionalLightingExecutor() = default;

	DirectionalLightingExecutor(uint32_t maxInstance);

	__NON_COPYABLE_CLASS(DirectionalLightingExecutor)

public:
	void reinitialize(uint32_t maxInstance);
	void draw_command(uint32_t InstanceCount) const;
	void write_to_buffer(uint32_t index, const DirectionalLightData& lightData_);

private:
	StructuredBuffer<DirectionalLightData> lightData;
};
