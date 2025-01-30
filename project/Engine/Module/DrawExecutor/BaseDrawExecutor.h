#pragma once

#include <memory>
#include <string>

#include <Library/Utility/Tools/ConstructorMacro.h>

template<typename ResourceType>
class BaseDrawExecutor {
public:
	BaseDrawExecutor() = default;
	virtual ~BaseDrawExecutor() = default;

	BaseDrawExecutor(const std::string& name, uint32_t maxInstance);

	__NON_COPYABLE_CLASS(BaseDrawExecutor)

public:
	virtual void reinitialize(const std::string& name, uint32_t maxInstance) = 0;
	virtual void draw_command(uint32_t InstanceCount) const = 0;

protected:
	std::shared_ptr<const ResourceType> resource;
};

template<typename ResourceType>
inline BaseDrawExecutor<ResourceType>::BaseDrawExecutor(const std::string& name, uint32_t maxInstance) {
	reinitialize(name, maxInstance);
}

#define __DRAW_EXECUTOR_CLASS(class_name) \
class_name(const std::string& name, uint32_t maxInstance) {\
	reinitialize(name, maxInstance);\
};\
__NON_COPYABLE_CLASS(class_name)\
