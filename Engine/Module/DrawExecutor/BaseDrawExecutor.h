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
	u32 max_instance() const { return maxInstance; }
	u32 count() const { return instanceCounter; }

protected:
	u32 maxInstance{ 0 };
	u32 instanceCounter{ 0 };
};

template<class InstanceType>
inline void BaseDrawExecutor<InstanceType>::begin() {
	instanceCounter = 0;
}
