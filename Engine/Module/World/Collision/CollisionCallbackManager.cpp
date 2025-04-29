#include "CollisionCallbackManager.h"

void CollisionCallbackManager::begin() {
	for (auto itr = collisionRecent.begin(); itr != collisionRecent.end();) {
		auto& [key, value] = *itr;
		// どちらかのcolliderが非アクティブ
		// 2フレーム連続で衝突がない
		if (!key.big()->is_active() || !key.small()->is_active() ||
			value.none()) {
			// 記録を削除
			itr = collisionRecent.erase(itr);
		}
		else {
			value <<= 1;
			++itr;
		}
	}
}

void CollisionCallbackManager::callback(CallbackInfo lhs, CallbackInfo rhs, bool result) {
	CallbackMapKey callbackKey = CallbackMapKey(lhs->group(), rhs->group());
	if (!callbackFunctions.contains(callbackKey)) {
		return;
	}
	const CallbackFunctions& callbackFunction = callbackFunctions.at(callbackKey);

	CollisionRecentKeyType recentKey = CollisionRecentKeyType(lhs, rhs);
	if (!collisionRecent.contains(recentKey)) {
		collisionRecent.emplace(recentKey, "00");
	}

	auto& collisionObject = collisionRecent.at(recentKey);

	collisionObject.set(0, result);
	u32 state = collisionObject.to_ulong();
	// このフレーム衝突しているか
	switch (state) {
	case 0b00:
		// 前フレームも衝突していない
		break;
	case 0b01:
		// 今はじめて衝突
			// Enter判定
		if (callbackFunction.onEnter) {
			callbackFunction.onEnter(lhs, rhs);
		}
		else if (callbackFunction.onContinue) {
			callbackFunction.onContinue(lhs, rhs);
		}
		break;
	case 0b10:
		// Exit
		if (callbackFunction.onExit) {
			callbackFunction.onExit(lhs, rhs);
		}
		else if (callbackFunction.onContinue) {
			callbackFunction.onContinue(lhs, rhs);
		}
		break;
	case 0b11:
		// 連続衝突
		if (callbackFunction.onContinue) {
			callbackFunction.onContinue(lhs, rhs);
		}
		break;
	default:
		break;
	}
}
