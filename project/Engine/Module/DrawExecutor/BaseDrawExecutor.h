#pragma once

#include <memory>

#include <Library/Utility/Tools/ConstructorMacro.h>

class BaseDrawExecutor {
public:
	BaseDrawExecutor() = default;
	virtual ~BaseDrawExecutor() = default;

	__CLASS_NON_COPYABLE(BaseDrawExecutor)

public:
	void begin();
	virtual void draw_command() const = 0;

public:
	uint32_t max_instance() const { return maxInstance; }
	uint32_t count() const { return instanceCounter; }

protected:
	uint32_t maxInstance{ 0 };
	uint32_t instanceCounter{ 0 };
};
