#pragma once

#include "Engine/Module/World/Collision/CollisionCallbackManager.h"

class CallbackManagerDemo final : public CollisionCallbackManager {
public:
	CallbackManagerDemo();
	~CallbackManagerDemo() = default;

	__NON_COPYABLE_CLASS(CallbackManagerDemo)

public:
	static void SetColor(__CALLBACK_ARGUMENT_DEFAULT(lhs, rhs), Color color);
};
