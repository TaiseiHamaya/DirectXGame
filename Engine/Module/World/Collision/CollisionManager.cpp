#include "CollisionManager.h"

#include <ranges>

#include <Library/Utility/Tools/SmartPointer.h>

#include "Engine/Module/World/Collision/CollisionFunctions.h"
#include "Engine/Assets/PrimitiveGeometry/PrimitiveGeometryLibrary.h"

CollisionManager::CollisionManager() {
#ifdef DEBUG_FEATURES_ENABLE

	sphereDebugDrawExecutor = eps::CreateUnique<PrimitiveGeometryDrawExecutor>(
		PrimitiveGeometryLibrary::GetPrimitiveGeometry("SphereCollider"), 1024
	);
	aabbDebugDrawExecutor = eps::CreateUnique<PrimitiveGeometryDrawExecutor>(
		PrimitiveGeometryLibrary::GetPrimitiveGeometry("AABBCollider"), 1024
	);

#endif // _DEBUG
}

void CollisionManager::begin() {
	if (collisionCallbackManager) {
		collisionCallbackManager->begin();
	}
}

void CollisionManager::update() {
	// 解放済み要素の削除
	for (auto itr = colliderList.begin(); itr != colliderList.end();) {
		bool isEraseList{ true };
		// 削除処理
		isEraseList = erase_expired(itr->second.sphereColliders) && isEraseList;
		isEraseList = erase_expired(itr->second.aabbColliders) && isEraseList;
		// 要素が空の場合リストから名前を削除
		if (isEraseList) {
#ifdef DEBUG_FEATURES_ENABLE
			keyList.erase(itr->first);
#endif // _DEBUG
			itr = colliderList.erase(itr);
		}
		else {
			++itr;
		}
	}
}

void CollisionManager::collision(const std::string& groupName1, const std::string& groupName2) {
	// Listに存在しない名前の場合判定しない
	if (!(colliderList.contains(groupName1) && colliderList.contains(groupName2))) {
		return;
	}
	auto& group1 = colliderList.at(groupName1);
	auto& group2 = colliderList.at(groupName2);

	test_colliders(group1.sphereColliders, group2.sphereColliders);
	test_colliders(group1.aabbColliders, group2.sphereColliders);
	test_colliders(group1.aabbColliders, group2.aabbColliders);
	test_colliders(group1.sphereColliders, group2.aabbColliders);
}

template<std::derived_from<BaseCollider> ColliderType>
bool CollisionManager::erase_expired(std::list<std::weak_ptr<ColliderType>>& colliders) {
	for (auto itr = colliders.begin(); itr != colliders.end();) {
		if (itr->expired()) {
			itr = colliders.erase(itr);
		}
		else {
			++itr;
		}
	}

	if (colliders.empty()) {
		return true;
	}
	return false;
}

template<std::derived_from<BaseCollider> LColliderType, std::derived_from<BaseCollider> RColliderType>
void CollisionManager::test_colliders(const std::list<std::weak_ptr<LColliderType>>& lhs, const std::list<std::weak_ptr<RColliderType>>& rhs) {
	for (const std::weak_ptr<LColliderType>& colliderL : lhs) {
		auto lLocked = colliderL.lock();
		if (!lLocked->is_active()) {
			continue;
		}
		for (const std::weak_ptr<RColliderType>& colliderR : rhs) {
			auto rLocked = colliderR.lock();
			if (!rLocked->is_active()) {
				continue;
			}
			if constexpr (std::is_same_v<LColliderType, RColliderType>) {
				if (lLocked == rLocked) {
					break;
				}
			}
			bool result = Collision(*lLocked, *rLocked);
			collisionCallbackManager->callback(
				lLocked.get(),
				rLocked.get(),
				result
			);
		}
	}
}

#ifdef DEBUG_FEATURES_ENABLE

#include <format>
#include <imgui.h>

void CollisionManager::debug_gui() {
	ImGui::Checkbox("ShowDebugDraw", &isShowDebugDraw);
	if (ImGui::TreeNode(std::format("ColliderList##{:}", (void*)this).c_str())) {
		for (auto& name : keyList) {
			if (colliderList.contains(name)) {
				auto& list = colliderList.at(name);
				ImGui::Text(
					std::format("{} : {}", name,
						list.aabbColliders.size() + list.sphereColliders.size()).c_str()
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

	sphereDebugDrawExecutor->begin();
	aabbDebugDrawExecutor->begin();

	for (const Colliders& colliders : colliderList | std::views::values) {
		for (const std::weak_ptr<SphereCollider>& sphereCollider : colliders.sphereColliders) {
			sphereDebugDrawExecutor->write_to_buffer(sphereCollider.lock()->debug_matrix());
		}

		for (const std::weak_ptr<AABBCollider>& aabbCollider : colliders.aabbColliders) {
			aabbDebugDrawExecutor->write_to_buffer(aabbCollider.lock()->debug_matrix());
		}
	}

	sphereDebugDrawExecutor->draw_command();
	aabbDebugDrawExecutor->draw_command();
}
#endif // _DEBUG
