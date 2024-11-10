#include "CollisionCallbackManager.h"

void CollisionCallbackManager::begin() {
	for (auto itr = collisionRecent.begin(); itr != collisionRecent.end();) {
		auto& key = itr->first;
		auto& value = itr->second;
		// どちらかのcolliderが非アクティブ
		// 2フレーム連続で衝突がない
		if (!key.big()->is_active() || !key.small()->is_active() ||
			value.none()) {
			// 記録を削除
			itr = collisionRecent.erase(itr);
		}
		else {
			itr->second <<= 1;
			++itr;
		}
	}
}

void CollisionCallbackManager::callback(const CallbackInfo& lhs, const CallbackInfo& rhs, bool result) {
	SortedPair<std::string> callbackKey = SortedPair<std::string>(lhs.first, rhs.first);
	if (!callbackFunctions.contains(callbackKey)) {
		return;
	}
	const CallbackFunctions& callbackFunction = callbackFunctions.at(callbackKey);

	CollisionRecentKeyType recentKey = CollisionRecentKeyType(lhs.second, rhs.second);
	if (!collisionRecent.contains(recentKey)) {
		collisionRecent.emplace(recentKey, "00");
	}

	auto& collisionObject = collisionRecent.at(recentKey);

	collisionObject.set(0, result);
	unsigned long state = collisionObject.to_ulong();
	// このフレーム衝突しているか
	switch (state) {
	case 0b00:
		// 前フレームも衝突していない
		break;
	case 0b01:
		// 今はじめて衝突
			// Enter判定
		if (callbackFunction.onEnter) {
			callbackFunction.onEnter(lhs.second, rhs.second);
		}
		else if (callbackFunction.onContinue) {
			callbackFunction.onContinue(lhs.second, rhs.second);
		}
		break;
	case 0b10:
		// Exit
		if (callbackFunction.onExit) {
			callbackFunction.onExit(lhs.second, rhs.second);
		}
		else if (callbackFunction.onContinue) {
			callbackFunction.onContinue(lhs.second, rhs.second);
		}
		break;
	case 0b11:
		// 連続衝突
		if (callbackFunction.onContinue) {
			callbackFunction.onContinue(lhs.second, rhs.second);
		}
		break;
	default:
		break;
	}
}
