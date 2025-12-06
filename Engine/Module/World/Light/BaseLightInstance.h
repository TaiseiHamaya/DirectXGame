#pragma once

#include "Engine/Module/World/WorldInstance/WorldInstance.h"

namespace szg {

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

	void set_influence_layer(u32 layer) { layerMask = layer; }

	u32 influence_layer() const { return layerMask; }

protected:
	T lightData;
	u32 layerMask;
};

}; // szg
