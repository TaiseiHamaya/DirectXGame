#pragma once

#include <algorithm>

#include "Engine/Module/World/Collision/Collider/SphereCollider.h"

#include "Engine/Module/World/Collision/Collider/CapsuleCollider.h"

bool Collision(SphereCollider* lhs, SphereCollider* rhs) {
	Vector3 distance = lhs->world_position() - rhs->world_position();
	float range = lhs->get_radius() + rhs->get_radius();
	return distance.length() <= range;
}

bool Collision(const CapsuleCollider* lhs, SphereCollider* rhs) {
	// ***基本的な処理は線と点のコリジョン***

	// 各ワールド座標を取得
	const Vector3& capsuleWorldPosition = lhs->world_position();
	const Vector3& sphereWorldPosition = rhs->world_position();
	
	// カプセルの向きを取得
	Vector3 direction = lhs->get_direction();
	// 始点取得
	Vector3 capsuleLengthOffset = -direction * lhs->get_length() / 2;
	Vector3 origin = capsuleWorldPosition + capsuleLengthOffset;
	// 球の座標系をカプセルに合わせる
	Vector3 spherePositionCapsuleLocal = sphereWorldPosition - origin;
	// カプセル線と球の最近点を取得
	float projection = Vector3::Projection(spherePositionCapsuleLocal, direction).length();
	Vector3 nearest = direction * std::clamp(projection, 0.0f, lhs->get_length());

	// 最近点との距離を算出
	Vector3 distance = spherePositionCapsuleLocal - nearest;

	// 許容範囲はカプセルと球の半径の合計
	float range = lhs->get_radius() + rhs->get_radius();
	return distance.length() <= range;
};