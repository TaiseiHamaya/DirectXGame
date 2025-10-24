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

public:
	/// <summary>
	/// ライトデータの不変参照
	/// </summary>
	/// <returns></returns>
	const T& light_data_imm() const { return lightData; };

	/// <summary>
	/// ライトデータの可変参照
	/// </summary>
	/// <returns></returns>
	T& light_data_mut() { return lightData; };

	u32 influence_layer() const { return layerMask; }

protected:
	T lightData;
	u32 layerMask;
};
