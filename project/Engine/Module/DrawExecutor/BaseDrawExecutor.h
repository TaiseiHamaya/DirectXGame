#pragma once

#include <Library/Utility/Template/Reference.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

template<class T>
class BaseDrawExecutor {
public:
	using InstanceType = T;
public:
	BaseDrawExecutor() = default;
	virtual ~BaseDrawExecutor() = default;

	__CLASS_NON_COPYABLE(BaseDrawExecutor)

public:
	void begin();
	virtual void draw_command() const = 0;
	virtual void write_to_buffer(Reference<const InstanceType>) = 0;

public:
	uint32_t max_instance() const { return maxInstance; }
	uint32_t count() const { return instanceCounter; }

protected:
	uint32_t maxInstance{ 0 };
	uint32_t instanceCounter{ 0 };
};

template<class InstanceType>
inline void BaseDrawExecutor<InstanceType>::begin() {
	instanceCounter = 0;
}
