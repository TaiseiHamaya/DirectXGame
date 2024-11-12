#pragma once

#include "Engine/Module/World/Collision/CollisionCallbackManager.h"

#include <Library/Math/Color.h>

class CallbackManagerDemo final : public CollisionCallbackManager {
public:
	CallbackManagerDemo();
	~CallbackManagerDemo() = default;

	__NON_COPYABLE_CLASS(CallbackManagerDemo)

public:
	static void SetColor(BaseCollider* const lhs, BaseCollider* const rhs, Color color);
};
