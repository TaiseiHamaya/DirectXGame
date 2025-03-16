#pragma once

#include "../BaseDrawExecutor.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/StructuredBuffer/StructuredBuffer.h"

struct DirectionalLightData;

class DirectionalLightingExecutor final : public BaseDrawExecutor {
public:
	DirectionalLightingExecutor() = default;
	~DirectionalLightingExecutor() = default;

	DirectionalLightingExecutor(uint32_t maxInstance);

	__CLASS_NON_COPYABLE(DirectionalLightingExecutor)

public:
	void reinitialize(uint32_t maxInstance);
	void draw_command() const override;
	void write_to_buffer(const DirectionalLightData& lightData_);

private:
	StructuredBuffer<DirectionalLightData> lightData;
};
