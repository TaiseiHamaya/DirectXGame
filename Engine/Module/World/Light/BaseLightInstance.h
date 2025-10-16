#pragma once

#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"
#include "Engine/GraphicsAPI/DirectX/DxResource/ConstantBuffer/ConstantBuffer.h"
#include "Engine/Module/World/WorldInstance/WorldInstance.h"

template<class T>
class BaseLightInstance : public WorldInstance {
public:
	BaseLightInstance() = default;
	virtual ~BaseLightInstance() = default;

	BaseLightInstance(const BaseLightInstance&) = delete;
	BaseLightInstance& operator=(const BaseLightInstance&&) = delete;
	BaseLightInstance(BaseLightInstance&&) = default;
	BaseLightInstance& operator=(BaseLightInstance&&) = default;

public:
	virtual void transfer() = 0;

public:
	const T& light_data_imm() const { return lightData; };
	T& light_data_mut() { return lightData; };

protected:
	T lightData;
};
