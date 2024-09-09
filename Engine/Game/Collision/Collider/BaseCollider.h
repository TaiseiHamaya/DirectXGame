#pragma once

#include <string>
#include <bitset>
#include <memory>
#include <functional>
#include <unordered_map>

#include "Engine/Game/Transform3D/Transform3D.h"
#include "Engine/Game/Hierarchy/Hierarchy.h"

#ifdef _DEBUG
#include <Engine/Game/GameObject/GameObject.h>
#endif // _DEBUG

class BaseCollider  {
public:
	BaseCollider();
	~BaseCollider() = default;

public:
	virtual void initialize() = 0;
	void begin();
	virtual void update();
	void collision(const BaseCollider* const collider, bool result);

public:
	Hierarchy& get_hierarchy() const;
	Transform3D& get_transform() const;
	Vector3 world_position() const;
	virtual constexpr std::string type() const = 0;
	const std::string& group() const noexcept;

public:
	void set_on_collision(std::function<void(const BaseCollider* const)> function);
	void set_on_collision_enter(std::function<void(const BaseCollider* const)> function);
	void set_on_collision_exit(std::function<void(const BaseCollider* const)> function);

	void set_group_name(const std::string& name);

private:
	std::unique_ptr<Transform3D> transform;
	const std::string* groupName;

protected:
	std::unique_ptr<Hierarchy> hierarchy;

private:
	Matrix4x4 worldMatrix;

	std::function<void(const BaseCollider* const)> onCollisionFunc;
	std::function<void(const BaseCollider* const)> onCollisionEnterFunc;
	std::function<void(const BaseCollider* const)> onCollisionExitFunc;

	std::unordered_map<const BaseCollider*, std::bitset<2>> collisionMap;

#ifdef _DEBUG
public:
	GameObject& get_collider_drawer() const;

protected:
	std::unique_ptr<GameObject> colliderDrawer;
#endif // _DEBUG
};

/*
Colliderに関する仕様メモ

CallBack関数を設定すると、衝突時にbindしたが呼び出される
Enter, Exitが設定されていなく衝突した場合、OnCollisionが呼び出される

この判定はColliderインスタンスごとに判定が行われる

*/