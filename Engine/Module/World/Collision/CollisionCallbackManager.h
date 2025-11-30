#pragma once

#include <bitset>
#include <functional>
#include <string>
#include <unordered_map>

#include <Library/Utility/Template/SortedPair.h>
#include <Library/Utility/Template/Reference.h>
#include <Library/Utility/Tools/ConstructorMacro.h>

#include "Collider/BaseCollider.h"

class CollisionCallbackManager {
protected:
	using CallbackTarget = Reference<BaseCollider>; // 対象
	using CollisionRecentKeyType = SortedPair<CallbackTarget>; // 衝突管理キー
	using CallbackMapKey = SortedPair<const std::string>; //  
	// コールバック関数
	struct CallbackFunctions { 
		std::function<void(CallbackTarget, CallbackTarget)> onContinue;
		std::function<void(CallbackTarget, CallbackTarget)> onEnter;
		std::function<void(CallbackTarget, CallbackTarget)> onExit;
	};

public:
	CollisionCallbackManager() = default;
	virtual ~CollisionCallbackManager() = default;

	__CLASS_NON_COPYABLE(CollisionCallbackManager)

public:
	void begin_callback();
	void remove_marked_destroy();
	void callback(CallbackTarget lhs, CallbackTarget rhs, bool result);

protected:
	std::unordered_map<CallbackMapKey, CallbackFunctions> callbackFunctions;

private:
	std::unordered_map<CollisionRecentKeyType, std::bitset<2>> collisionRecent;
};

#define __CALLBACK_PLACEHOLDERS_12 std::placeholders::_1, std::placeholders::_2
#define __CALLBACK_ARGUMENT_DEFAULT(first, second) CallbackTarget first, CallbackTarget second

/*
Callbackメモ

callback用関数で呼び出される引数の順序は、
CollisionManager::collisionの引数順序と同じ

*/