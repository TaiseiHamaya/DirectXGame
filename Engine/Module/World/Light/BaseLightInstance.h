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

protected:
	T lightData;
};
