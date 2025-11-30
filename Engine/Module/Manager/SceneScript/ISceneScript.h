#pragma once

#include <Library/Utility/Tools/ConstructorMacro.h>

class ISceneScript {
public:
	ISceneScript() = default;
	virtual ~ISceneScript() = default;

	__CLASS_NON_COPYABLE(ISceneScript)

public:
	virtual void finalize() {};

	virtual void prev_update() {};
	virtual void post_update() {};
};
