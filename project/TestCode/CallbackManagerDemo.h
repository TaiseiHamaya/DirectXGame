#pragma once

#include "Engine/Module/World/Collision/CollisionCallbackManager.h"

class CallbackManagerDemo final : public CollisionCallbackManager {
public:
	CallbackManagerDemo();
	~CallbackManagerDemo() = default;

	__NON_COPYABLE_CLASS(CallbackManagerDemo)

public:
	static void SetColor(const BaseCollider* const lhs, const BaseCollider* const rhs, Color color);
};
