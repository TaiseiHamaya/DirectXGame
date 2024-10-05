#include "CollisionManager.h"

#include <ranges>

#include "Engine/Module/Collision/CollisionFunctions.h"

void CollisionManager::update() {
	// 解放済み要素の削除
	for (auto itr = colliderList.begin(); itr != colliderList.end();) {
		if (itr->second.expired()) {
#ifdef _DEBUG
			if (colliderList.count(itr->first) == 1) {
				keyList.erase(itr->first);
			}
#endif // _DEBUG
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
	using ColliderIterator = std::unordered_multimap<std::string, std::weak_ptr<BaseCollider>>::const_iterator;
	auto group1Range = colliderList.equal_range(groupName1);
	auto group2Range = colliderList.equal_range(groupName2);
	ColliderIterator group2;
	for (ColliderIterator& group1 = group1Range.first; group1 != group1Range.second; ++group1) {
		// 同じグループのときは重複させないようにgroup1の次を参照させる
		if (groupName1 == groupName2) {
			group2 = std::next(group1);
		}
		else {
			group2 = group2Range.first;
		}

		for (; group2 != group2Range.second; ++group2) {
			test_collision(group1->second.lock(), group2->second.lock());
		}
	}
}

void CollisionManager::register_collider(const std::string& groupName, const std::weak_ptr<BaseCollider>& collider) {
	auto newCollider = colliderList.emplace(groupName, collider);
	collider.lock()->set_group_name(newCollider->first);
#ifdef _DEBUG
	keyList.insert(groupName);
#endif // _DEBUG
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

#include <format>
#include <imgui.h>

void CollisionManager::debug_gui() {
	ImGui::Checkbox("ShowDebugDraw", &isShowDebugDraw);
	if (ImGui::TreeNode(std::format("ColliderList##{:}", (void*)this).c_str())) {
		for (auto& name : keyList) {
			if (colliderList.contains(name)) {
				ImGui::Text(
					std::format("{} : {}", name, colliderList.count(name)).c_str()
				);
			}
		}
		ImGui::TreePop();
	}
}

void CollisionManager::debug_draw3d() {
	if (!isShowDebugDraw) {
		return;
	}
	for (const auto& list : colliderList | std::views::values) {
		auto listLocked = list.lock();
		if (listLocked) {
			auto& drawer = listLocked->get_collider_drawer();
			drawer.begin_rendering();
			drawer.draw();
		}
	}
}

#include "Engine/Module/PolygonMesh/PolygonMeshManager.h"

void CollisionManager::LoadDebugDrawMesh() {
	PolygonMeshManager::RegisterLoadQue("./EngineResources/Models/Collider/Sphere", "SphereCollider.obj");
}
#endif // _DEBUG
