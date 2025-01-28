#pragma once

#include "Engine/Module/World/WorldInstance/WorldInstance.h"
#include "Engine/Rendering/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/Rendering/DirectX/DirectXResourceObject/ConstantBuffer/ConstantBuffer.h"

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
	virtual void update() {};

	virtual void begin_rendering() = 0;

public:
	const T& light_data() const { return lightData; };

protected:
	T lightData;
};
