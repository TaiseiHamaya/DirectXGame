#pragma once

#include <Library/Utility/Tools/ConstructorMacro.h>

class ISceneScript {
public:
	ISceneScript() = default;
	virtual ~ISceneScript() = default;

	__CLASS_NON_COPYABLE(ISceneScript)

public:
	virtual void prev_update() = 0;
	virtual void post_update() = 0;
};
