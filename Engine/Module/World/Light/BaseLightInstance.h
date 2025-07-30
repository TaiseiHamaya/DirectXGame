#pragma once

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
	const T& light_data() const { return lightData; };

	u32 influence_layer() const { return layerMask; }

protected:
	T lightData;
	u32 layerMask;
};
