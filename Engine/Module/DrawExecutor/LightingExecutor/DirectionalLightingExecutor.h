#pragma once

#include "../BaseDrawExecutor.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/StructuredBuffer/StructuredBuffer.h"
#include "Engine/Module/World/Light/DirectionalLight/DirectionalLightInstance.h"

class DirectionalLightingExecutor final : public BaseDrawExecutor<DirectionalLightInstance> {
public:
	DirectionalLightingExecutor() = default;
	~DirectionalLightingExecutor() = default;

	DirectionalLightingExecutor(u32 maxInstance);

	__CLASS_NON_COPYABLE(DirectionalLightingExecutor)

public:
	void reinitialize(u32 maxInstance);
	void draw_command() const override;
	void set_command(u32 paramIndex) const;
	void write_to_buffer(Reference<const DirectionalLightInstance> instance) override;

private:
	StructuredBuffer<DirectionalLightData> lightData;
};
