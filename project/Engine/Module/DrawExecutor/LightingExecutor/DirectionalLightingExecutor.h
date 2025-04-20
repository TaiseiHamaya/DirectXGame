#pragma once

#include "../BaseDrawExecutor.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/StructuredBuffer/StructuredBuffer.h"

class DirectionalLightInstance;
struct DirectionalLightData;

class DirectionalLightingExecutor final : public BaseDrawExecutor<DirectionalLightInstance> {
public:
	DirectionalLightingExecutor() = default;
	~DirectionalLightingExecutor() = default;

	DirectionalLightingExecutor(uint32_t maxInstance);

	__CLASS_NON_COPYABLE(DirectionalLightingExecutor)

public:
	void reinitialize(uint32_t maxInstance);
	void draw_command() const override;
	void set_command(uint32_t paramIndex) const;
	void write_to_buffer(Reference<const DirectionalLightInstance> instance) override;

private:
	StructuredBuffer<DirectionalLightData> lightData;
};
