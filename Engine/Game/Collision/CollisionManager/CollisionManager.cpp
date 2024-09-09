#include "CollisionManager.h"

#include <ranges>

#include "Engine/Game/Collision/CollisionManager/CollisionFunctions.h"

void CollisionManager::update() {
	// 解放済み要素の削除
	for (auto itr = colliderList.begin(); itr != colliderList.end();) {
		if (itr->second.expired()) {
			itr = colliderList.erase(itr);
		}
		else {
			++itr;
		}
	}
	// 更新処理
	for (const auto& list : colliderList | std::views::values) {
		auto colliderLocked = list.lock();
		if (colliderLocked) {
			colliderLocked->begin();
			colliderLocked->update();
		}
	}
}

void CollisionManager::collision(const std::string& groupName1, const std::string& groupName2) {
	auto&& group1Range = colliderList.equal_range(groupName1);
	for (auto& group1 = group1Range.first; group1 != group1Range.second; ++group1) {
		auto&& group2Range = colliderList.equal_range(groupName2);
		auto& group2 = group2Range.first;
		// 同じグループのときは重複させないようにgroup2のiteratorを進める
		if (groupName1 == groupName2) {
			while (group1 != group2) {
				++group2;
			}
			++group2;
		}
		for (; group2 != group2Range.second; ++group2) {
			test_collision(group1->second.lock(), group2->second.lock());
		}
	}
}

void CollisionManager::register_collider(const std::string& groupName, const std::weak_ptr<BaseCollider>& collider) {
	colliderList.emplace(groupName, collider);
}

void CollisionManager::test_collision(const std::shared_ptr<BaseCollider>& test1, const std::shared_ptr<BaseCollider>& test2) {
	// 衝突対象が同じ場合は判定しない
	if (test1 == test2) {
		return;
	}
	std::string type1 = test1->type();
	std::string type2 = test2->type();

	bool result = false;

	if (type1 == "Sphere") {
		auto downed1 = static_cast<SphereCollider*>(test1.get());
		if (type2 == "Sphere") {
			auto downed2 = static_cast<SphereCollider*>(test2.get());
			if (Collision(downed1, downed2)) {
				result = true;
			}
		}
	}

	test1->collision(test2.get(), result);
	test2->collision(test1.get(), result);
}

#ifdef _DEBUG
void CollisionManager::debug_draw3d(const Camera3D& camera) {
	for (const auto& list : colliderList | std::views::values) {
		auto listLocked = list.lock();
		if (listLocked) {
			auto& drawer = listLocked->get_collider_drawer();
			drawer.begin_rendering(camera);
			drawer.draw();
		}
	}
}

#include "Engine/Game/Managers/PolygonMeshManager/PolygonMeshManager.h"

void CollisionManager::LoadDebugDrawMesh() {
	PolygonMeshManager::RegisterLoadQue("./Engine/Resources/Collider/Sphere", "SphereCollider.obj");
}
#endif // _DEBUG
