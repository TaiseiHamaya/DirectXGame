#pragma once

#include <bitset>
#include <functional>
#include <string>
#include <unordered_map>
#include <utility>

#include <Library/Utility/Template/SortedPair.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

#include "Collider/BaseCollider.h"

class CollisionCallbackManager {
protected:
	using CallbackMapKey = SortedPair<const std::string>;
	struct CallbackFunctions {
		std::function<void(BaseCollider* const, BaseCollider* const)> onContinue;
		std::function<void(BaseCollider* const, BaseCollider* const)> onEnter;
		std::function<void(BaseCollider* const, BaseCollider* const)> onExit;
	};

private:
	using CallbackInfo = std::pair<const std::string&, BaseCollider* const>;
	using CollisionRecentKeyType = SortedPair<BaseCollider* const>;

public:
	CollisionCallbackManager() = default;
	virtual ~CollisionCallbackManager() = default;

	__NON_COPYABLE_CLASS(CollisionCallbackManager)

public:
	void begin();
	void callback(CallbackInfo lhs, CallbackInfo rhs, bool result);

protected:
	std::unordered_map<CallbackMapKey, CallbackFunctions> callbackFunctions;

private:
	std::unordered_map<CollisionRecentKeyType, std::bitset<2>> collisionRecent;
};

#define __CALLBACK_PLACEHOLDERS_12 std::placeholders::_1, std::placeholders::_2
#define __CALLBACK_ARGUMENT_DEFAULT(first, second) BaseCollider* const first, BaseCollider* const second

/*
Callbackメモ

callback用関数で呼び出される引数の順序は、
CollisionManager::collisionの引数順序と同じ

*/