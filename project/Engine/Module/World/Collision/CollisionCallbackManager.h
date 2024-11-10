#pragma once

#include <bitset>
#include <functional>
#include <string>
#include <unordered_map>
#include <utility>

#include "Engine/Utility/Template/SortedPair.h"
#include "Engine/Utility/Tools/ConstructorMacro.h"

#include "Collider/BaseCollider.h"

class CollisionCallbackManager {
protected:
	using CallbackMapKey = SortedPair<std::string>;
	struct CallbackFunctions {
		std::function<void(const BaseCollider* const, const BaseCollider* const)> onContinue;
		std::function<void(const BaseCollider* const, const BaseCollider* const)> onEnter;
		std::function<void(const BaseCollider* const, const BaseCollider* const)> onExit;
	};

private:
	using CallbackInfo = std::pair<std::string, const BaseCollider*>;
	using CollisionRecentKeyType = SortedPair<const BaseCollider*>;

public:
	CollisionCallbackManager() = default;
	virtual ~CollisionCallbackManager() = default;

	__NON_COPYABLE_CLASS(CollisionCallbackManager)

public:
	void begin();
	void callback(const CallbackInfo& lhs, const CallbackInfo& rhs, bool result);

protected:
	std::unordered_map<CallbackMapKey, CallbackFunctions> callbackFunctions;

private:
	std::unordered_map<CollisionRecentKeyType, std::bitset<2>> collisionRecent;
};
