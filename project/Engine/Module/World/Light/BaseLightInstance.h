#pragma once

#include "Engine/Module/World/WorldInstance/WorldInstance.h"

#include "Engine/Rendering/DirectX/DirectXResourceObject/ConstantBuffer/ConstantBuffer.h"

#include "Engine/Rendering/DirectX/DirectXCommand/DirectXCommand.h"

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

	virtual void begin_rendering() {};

	void register_world(uint32_t parameterIndex) const;

protected:
	ConstantBuffer<T> lightBuffer;
};

template<class T>
inline void BaseLightInstance<T>::register_world(uint32_t parameterIndex) const {
	DirectXCommand::GetCommandList()->SetGraphicsRootConstantBufferView(
		parameterIndex,
		lightBuffer.get_resource()->GetGPUVirtualAddress()
	);
}
