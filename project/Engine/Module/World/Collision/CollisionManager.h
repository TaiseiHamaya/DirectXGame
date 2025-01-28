#pragma once

#include <list>
#include <memory>
#include <string>
#include <unordered_map>

#include "Collider/BaseCollider.h"
#include "CollisionCallbackManager.h"

class SphereCollider;
class AABBCollider;

#ifdef _DEBUG
#include <unordered_set>
#include "Engine/Module/DrawExecutor/PrimitiveGeometryDrawExecutor/PrimitiveGeometryDrawExecutor.h"
#endif // _DEBUG

class CollisionManager {
private:
	struct Colliders {
		std::list<std::weak_ptr<SphereCollider>> sphereColliders;
		std::list<std::weak_ptr<AABBCollider>> aabbColliders;
	};

public:
	CollisionManager();
	~CollisionManager() = default;

	CollisionManager(const CollisionManager&) = delete;
	CollisionManager(CollisionManager&&) = delete;

public:
	void begin();
	void update();
	void collision(const std::string& groupName1, const std::string& groupName2);

	template<std::derived_from<BaseCollider> ColliderType>
	void register_collider(const std::string& groupName, const std::shared_ptr<ColliderType>& collider);

private:
	template<std::derived_from<BaseCollider> ColliderType>
	bool erase_expired(std::list<std::weak_ptr<ColliderType>>& colliders);

	template<std::derived_from<BaseCollider> LColliderType, std::derived_from<BaseCollider> RColliderType>
	void test_colliders(
		const std::list<std::weak_ptr<LColliderType>>& lhs,
		const std::list<std::weak_ptr<RColliderType>>& rhs);

public:
	void set_callback_manager(std::unique_ptr<CollisionCallbackManager> manager) { collisionCallbackManager = std::move(manager); };

#ifdef _DEBUG
public:
	void debug_gui();
	void debug_draw3d();
#endif // _DEBUG

private:
	std::unique_ptr<CollisionCallbackManager> collisionCallbackManager;
	std::unordered_map<std::string, Colliders> colliderList;
	//std::unordered_multimap<std::string, std::weak_ptr<BaseCollider>> colliderList;

#ifdef _DEBUG
	bool isShowDebugDraw = true;
	std::unordered_set<std::string> keyList;

	std::unique_ptr<PrimitiveGeometryDrawExecutor> sphereDebugDrawExecutor;
	std::unique_ptr<PrimitiveGeometryDrawExecutor> aabbDebugDrawExecutor;
#endif // _DEBUG
};

template<std::derived_from<BaseCollider> ColliderType>
inline void CollisionManager::register_collider(const std::string& groupName, const std::shared_ptr<ColliderType>& collider) {
	Colliders& colliders = colliderList[groupName];
	if constexpr (std::is_same_v<ColliderType, SphereCollider>) {
		colliders.sphereColliders.emplace_back(collider);
	}
	else if constexpr (std::is_same_v<ColliderType, AABBCollider>) {
		colliders.aabbColliders.emplace_back(collider);
	}
	collider->set_group_name(colliderList.find(groupName)->first);
#ifdef _DEBUG
	keyList.insert(groupName);
#endif // _DEBUG
}
