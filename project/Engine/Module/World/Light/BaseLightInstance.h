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

	virtual void draw_deferred() const = 0;

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
